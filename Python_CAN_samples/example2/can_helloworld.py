#!/usr/bin/python3
#
#   CAN Helloworld
#   --------------
#
#   TEST
#   ----
#   $ mosquitto_sub -h 127.0.0.1 -p 7883 -t 'can_helloworld/#' -v
#
#   INFO
#   ----
#   https://python-can.readthedocs.io/en/master/interfaces/socketcan.html
#   http://www.steves-internet-guide.com/into-mqtt-python-client/
#

import paho.mqtt.client as mqtt
import can
import time

mqtt_host = "127.0.0.1"
mqtt_port = 7883
mqtt_keepalive = 60
mqtt_topic = "can_helloworld/data"

can_interface = 'can0'

print('Start CAN Helloworld...')

client = mqtt.Client()
client.connect(host=mqtt_host, port=mqtt_port, keepalive=mqtt_keepalive)

bus = can.interface.Bus(channel=can_interface, bustype='socketcan')

print('Waiting for CAN message...')
while True:
    message = bus.recv(10.0)  # Timeout in seconds.

    if message is None:
        print('Waiting for CAN message...')
    else:
        mqtt_data = '{ '
        mqtt_data += '"id":"' + hex(message.arbitration_id) + '", '
        #\"type\":\"EFF\"
        if message.is_extended_id is True:
            mqtt_data += '"type":"EFF", '
        else:
            mqtt_data += '"type":"SFF", '
        mqtt_data += '"len":' + str(message.dlc)
        if message.dlc > 0:
            mqtt_data += ', "data":' + str(list(bytearray(message.data)))
        mqtt_data += ' }'

        client.publish(topic=mqtt_topic, payload=mqtt_data, qos=0, retain=False)
        print('MQTT topic: ' + mqtt_topic)
        print('MQTT payload: ' + mqtt_data)
