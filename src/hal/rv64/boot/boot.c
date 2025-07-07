#include <kernel.h>

#include <hal/rv64/platform/device_tree.h>
#include <hal/uart.h>
#include <io/print.h>

void os_rta_rv64_c_entry(word hart_id, physical_address fdt_address)
{
    // initialize UART for logging
    os_rta_hal_uart_init(115200);
    HAL_LOG("UART initialized at 115200 baud rate");
    HAL_LOG("hart_id: %u", hart_id);
    HAL_LOG("fdt_address: 0x%lx", fdt_address);
    parse_dtb(fdt_address);

    int result = os_rta_kernel_main(hart_id, fdt_address, 0, 0);
}
