#ifndef _SSV_LOG_H_
#define _SSV_LOG_H_

#include <syslog.h>

extern int log_lvl;
void    log_open(char *exe, int daemon);
void    log_close(void);
void    log_printf(int lvl, char *text, ...) __attribute__ ((__format__ (__printf__, 2, 3)));

#endif
