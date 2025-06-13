#include <kernel.h>

void os_rta_rv64_c_entry(word hart_id, physical_address fdt_address)
{
    int result = os_rta_kernel_main(hart_id, fdt_address, 0, 0);
}
