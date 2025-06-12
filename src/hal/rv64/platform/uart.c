#include <hal/uart.h>

#include <hal/put_char.h>
#include <types.h>

#define OS_RTA_RV64_UART_BASE 0x10000000UL

#define OS_RTA_RV64_UART_THR 0x00
#define OS_RTA_RV64_UART_RBR 0x00
#define OS_RTA_RV64_UART_DLL 0x00
#define OS_RTA_RV64_UART_DLM 0x01
#define OS_RTA_RV64_UART_IER 0x01
#define OS_RTA_RV64_UART_FCR 0x02
#define OS_RTA_RV64_UART_LCR 0x03
#define OS_RTA_RV64_UART_LSR 0x05
#define OS_RTA_RV64_UART_LSR_DATA_READY 0x01
#define OS_RTA_RV64_UART_LSR_THR_EMPTY 0x20

void os_rta_hal_putchar(int c)
{
    while ((*((volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_LSR))
            & OS_RTA_RV64_UART_LSR_THR_EMPTY)
           == 0)
    {
    }

    *((volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_THR)) = c;
}

char os_rta_hal_input_char(void)
{
    while ((*((volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_LSR))
            & OS_RTA_RV64_UART_LSR_DATA_READY)
           == 0)
    {
        // busy wait
    }

    return *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_RBR);
}

void os_rta_hal_uart_init(int baud_rate)
{
    word divider = 1843200 / (baud_rate * 16);
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_IER) = 0;
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_LCR) = (1 << 7);
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_DLL) = divider;
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_DLM) = 0x00;
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_LCR) = (0x03);
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_FCR) = (0x01);
    *(volatile unsigned char *)(OS_RTA_RV64_UART_BASE + OS_RTA_RV64_UART_IER) = (0x01);
}
