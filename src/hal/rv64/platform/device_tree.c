#include <hal/rv64/platform/device_tree.h>

#include <io/print.h>

void parse_dtb(uintptr_t fdt_base)
{
    HAL_LOG("fdt_base: 0x%x", fdt_base);
    if (!fdt_base)
    {
        ERROR_LOG("Device Tree Base Address is NULL");
        return;
    }

    struct fdt_header *header = (struct fdt_header *)fdt_base;
    HAL_LOG("Device Tree Header:");
    HAL_LOG("Device Tree Base Address: 0x%x", fdt_base);
    HAL_LOG("Magic: 0x%X", header->magic);
    HAL_LOG("Version: %u.%u", header->version >> 16, header->version & 0xFFFF);
    HAL_LOG("Total Size: %u bytes", header->totalsize);
    HAL_LOG("Off Mem Rsv: %u bytes", header->off_mem_rsvmap);
    HAL_LOG("Off Dt Struct: %u bytes", header->off_dt_struct);
    HAL_LOG("Off Dt Strings: %u bytes", header->off_dt_strings);
    HAL_LOG("Version: %u.%u", header->version >> 16, header->version & 0xFFFF);
    HAL_LOG("Last Comp Version: %u.%u", header->last_comp_version >> 16, header->last_comp_version & 0xFFFF);
    HAL_LOG("Boot CPU ID: %u", header->boot_cpuid_phys);
    HAL_LOG("Size of Struct: %u bytes", header->size_dt_struct);
}
