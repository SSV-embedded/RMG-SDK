#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include "log.h"

int log_lvl = LOG_ERR;  /* Default log level */

void log_open(char *exe, int daemon) {
    if (log_lvl < LOG_EMERG)
        log_lvl = LOG_EMERG;
    if (log_lvl > LOG_DEBUG)
        log_lvl = LOG_DEBUG;
    openlog(exe, LOG_NDELAY | LOG_PID | (daemon == 0 ? LOG_PERROR : 0), LOG_DAEMON);
}

void log_close(void) {
    closelog();
}

void log_printf(int lvl, char *text, ...) {
    va_list args;

    va_start(args, text);
    if (lvl <= log_lvl) {
#if 1
        vsyslog(lvl, text, args);
#else
        vprintf(text, args);
#endif
    }
    va_end(args);
}
