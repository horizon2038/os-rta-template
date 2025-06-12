#ifndef OS_RTA_HAL_RV64_PLATFORM_PLIC_H
#define OS_RTA_HAL_RV64_PLATFORM_PLIC_H

#include <types.h>

void  os_rta_rv64_plic_init(void);
sword os_rta_rv64_plic_read_claim(void);
void  os_rta_rv64_plic_write_claim(sword irq);

#define PLIC_BASE 0x0C000000UL
#define PLIC_PRIORITY 0x00
#define PLIC_ENABLE 0x2080
#define PLIC_THRESHOLD 0x201000
#define PLIC_CLAIM 0x201004
#define PLIC_UART_IRQ 0x0A

#endif
