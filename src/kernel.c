#include <hal/init.h>
#include <hal/interrupt.h>
#include <hal/put_char.h>
#include <hal/timer.h>
#include <hal/uart.h>

#include <io/print.h>

static void os_rta_handle_timer(void)
{
    KERNEL_LOG("clock!\n");
}

static void os_rta_handle_interrupt(sword irq)
{
    KERNEL_LOG("interrupt %d!\n", irq);
}

static void os_rta_handle_kernel_call(sword irq)
{
    KERNEL_LOG("kernel call %d!\n", irq);
}

static void hal_init(void)
{
    KERNEL_LOG("initializing HAL...");
    os_rta_hal_init();
    os_rta_hal_uart_init(115200);
    os_rta_hal_timer_init(os_rta_handle_timer);
    os_rta_hal_interrupt_init();
    os_rta_hal_interrupt_register_handler(os_rta_handle_interrupt);
    os_rta_hal_kernel_call_init();
    os_rta_hal_kernel_call_register_handler(os_rta_handle_kernel_call);
}

static void kernel_init(void)
{
    KERNEL_LOG("initializing KERNEL...");
    KERNEL_LOG("KERNEL initialized successfully.");
}

void kernel_main(void)
{
    KERNEL_LOG("start OS RTA Kernel!");

    hal_init();
    kernel_init();

    for (;;)
    {
        // char c = os_rta_hal_input_char();
        // printf("%c", c);
    }
}
