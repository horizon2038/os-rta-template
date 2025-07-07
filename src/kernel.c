#include <kernel.h>

#include <hal/init.h>
#include <hal/interrupt.h>
#include <hal/put_char.h>
#include <hal/timer.h>
#include <hal/uart.h>

#include <io/print.h>

#include <hal/rv64/platform/device_tree.h>

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

const char os_rta_logo[]
    = "  ___  ____        ____ _____  _    \n"
      " / _ \\/ ___|      |  _ \\_   _|/ \\   \n"
      "| | | \\___ \\ _____| |_) || | / _ \\  \n"
      "| |_| |___) |_____|  _ < | |/ ___ \\ \n"
      " \\___/|____/      |_| \\_\\|_/_/   \\_\\\n"
      "                                    \n";

static void hal_init(void)
{
    KERNEL_LOG("initializing HAL...");
    os_rta_hal_init();
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

int os_rta_kernel_main(word a, word b, word c, word d)
{
    printf("%s", os_rta_logo);
    KERNEL_LOG("start OS RTA Kernel!");
    KERNEL_LOG("kernel main called with args: 0x%x, 0x%x, 0x%x, 0x%x", a, b, c, d);
    KERNEL_LOG("kernel_main: 0x%x\n", (void *)os_rta_kernel_main);

    hal_init();
    kernel_init();

    return 0;
}
