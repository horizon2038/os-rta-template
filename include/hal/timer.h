#ifndef OS_RTA_HAL_TIMER_H
#define OS_RTA_HAL_TIMER_H

typedef void (*timer_handler)(void);

void os_rta_hal_timer_init(timer_handler handler);

#endif
