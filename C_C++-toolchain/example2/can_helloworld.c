/*
    CAN Helloworld
    --------------

    TEST
    ----
    $ mosquitto_sub -h 127.0.0.1 -p 7883 -t 'can_helloworld/#' -v

    INFO
    ----
    https://www.kernel.org/doc/Documentation/networking/can.txt
    https://mosquitto.org/api/files/mosquitto-h.html
    https://www.beyondlogic.org/example-c-socketcan-code/
    https://github.com/craigpeacock/CAN-Examples
    https://github.com/linux-can/can-utils
    https://jsonformatter.curiousconcept.com
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <mosquitto.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "log.h"

struct {
    char *host;
    int port;
    int keepalive;
    int retain;
    int qos;
    int reconnect_delay;
    int reconnect_delay_max;
    char *topic;
} mqtt_config;

struct {
    char *device;
} can_config;

struct mosquitto *mqtt_client = NULL;

void mqtt_connectCallback(struct mosquitto *mq, void *obj, int result)
{
    log_printf(LOG_INFO, "MQTT: connected to broker %s:%d (%d)\n", mqtt_config.host, mqtt_config.port, result);
}

void mqtt_disconnectCallback(struct mosquitto *mq, void *obj, int result)
{
    log_printf(LOG_INFO, "MQTT: disconnected (%d)\n", result);
}

void mqtt_publishCallback(struct mosquitto *mq, void *obj, int mid)
{
    log_printf(LOG_DEBUG, "MQTT: publish mid %d done\n", mid);
}

int running = 1;

static void sighandler(int sign)
{
    static int forceexit = 0;

    if (forceexit != 0) {
        log_printf(LOG_INFO, "Exit.\n");
        exit(100);
    }
    forceexit = 1;

    log_printf(LOG_DEBUG, "received ctrl-c");
    running = 0;
}

int main(void)
{
    int rc, i, s, n;

    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    log_lvl = LOG_DEBUG;
    log_open("can_helloworld", 0);

    log_printf(LOG_INFO, "Startet.\n");

    /* using can device */
    can_config.device = "can0";

    /* mqtt setup */
    mqtt_config.host = "127.0.0.1";
    mqtt_config.port = 7883;
    mqtt_config.keepalive = 60; /* max disconnect time: 2*60 s = 120 s */
    mqtt_config.retain = 0;
    mqtt_config.qos = 0;
    mqtt_config.reconnect_delay = 10;
    mqtt_config.reconnect_delay_max = 30;
    mqtt_config.topic = "can_helloworld/data";

    /* install signal handler */
    signal(SIGINT, sighandler);

    /* mqtt */
    mosquitto_lib_init();

    mqtt_client = mosquitto_new(NULL, true, NULL); // autoid, clean, userdata
    if (!mqtt_client) {
        log_printf(LOG_ERR, "Error: can't create mosquitto session (%d)\n", errno);
        mosquitto_lib_cleanup();
        return 1;
    }

    mosquitto_connect_callback_set(mqtt_client, mqtt_connectCallback);
    mosquitto_disconnect_callback_set(mqtt_client, mqtt_disconnectCallback);
    mosquitto_publish_callback_set(mqtt_client, mqtt_publishCallback);
    mosquitto_reconnect_delay_set(mqtt_client, mqtt_config.reconnect_delay, mqtt_config.reconnect_delay_max, false);

    log_printf(LOG_INFO, "MQTT: connect to broker %s:%d ...\n", mqtt_config.host, mqtt_config.port);
    rc = mosquitto_connect(mqtt_client, mqtt_config.host, mqtt_config.port, mqtt_config.keepalive);
    if (rc) {
        log_printf(LOG_WARNING, "Can't connect to MQTT broker\n");
    }
    mosquitto_loop_start(mqtt_client);

    /* can */
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        log_printf(LOG_ERR, "socket(): %s\n", strerror(errno));
        running = 0;
    }

    log_printf(LOG_INFO, "Using CAN device: %s\n", can_config.device);
    strcpy(ifr.ifr_name, can_config.device);
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        log_printf(LOG_ERR, "bind(): %s\n", strerror(errno));
        running = 0;
    }

    if (running) {
        log_printf(LOG_INFO, "Running.\n");
    }

    /* main loop */
    while (running) {
        char buf[128], payload[512];
        int mid;

        n = read(s, &frame, sizeof(struct can_frame));
        if (n < 0) {
            log_printf(LOG_ERR, "read(): %s\n", strerror(errno));
            break;
        }
        if (!running) {
            break;
        }

        if (frame.can_id & CAN_EFF_FLAG) {
            snprintf(buf, sizeof(buf), "{ \"id\":\"0x%08X\", \"type\":\"EFF\", \"len\":%d", frame.can_id & CAN_EFF_MASK, frame.can_dlc);
        } else {
            snprintf(buf, sizeof(buf), "{ \"id\":\"0x%03X\", \"type\":\"SFF\", \"len\":%d", frame.can_id & CAN_SFF_MASK, frame.can_dlc);
        }
        strncpy(payload, buf, sizeof(payload));

        if (frame.can_dlc > 0) {
            strcat(payload, ", \"data\":[");
            if (frame.can_dlc > 1) {
                for (i = 0; i < (frame.can_dlc - 1); i++) {
                    snprintf(buf, sizeof(buf), "%d,", frame.data[i]);
                    strcat(payload, buf);
                }
            }
            snprintf(buf, sizeof(buf), "%d]", frame.data[frame.can_dlc - 1]);
            strcat(payload, buf);
        }

        strcat(payload, " }");
        payload[sizeof(payload)] = 0;

        mosquitto_publish(mqtt_client, &mid, "can_helloworld/data", strlen(payload), payload, mqtt_config.qos, mqtt_config.retain);
        log_printf(LOG_DEBUG, "MQTT: publish: mid %d\n", mid);
        log_printf(LOG_DEBUG, "MQTT: topic: %s\n", mqtt_config.topic);
        log_printf(LOG_DEBUG, "MQTT: payload: %s\n", payload);
    }

    /* main loop ends */
    if (close(s) < 0) {
        log_printf(LOG_ERR, "close(): %s\n", strerror(errno));
    }

    mosquitto_disconnect(mqtt_client);
    mosquitto_loop_stop(mqtt_client, false);
    mosquitto_destroy(mqtt_client);
    mosquitto_lib_cleanup();

    log_printf(LOG_INFO, "Stopped.\n");

    return 0;
}
