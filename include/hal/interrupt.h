#ifndef OS_RTA_HAL_INTERRUPT_H
#define OS_RTA_HAL_INTERRUPT_H

#include <types.h>

typedef void (*interrupt_handler)(sword irq);
typedef void (*kernel_call_handler)(sword irq);

void os_rta_hal_interrupt_init(void);
void os_rta_hal_interrupt_register_handler(interrupt_handler handler);
void os_rta_hal_kernel_call_init(void);
void os_rta_hal_kernel_call_register_handler(kernel_call_handler handler);

#define HAL_RESERVED_IRQ 0x10000
#define HAL_UART_IRQ (HAL_RESERVED_IRQ + 1)

#endif
