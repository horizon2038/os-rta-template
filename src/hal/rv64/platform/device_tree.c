#include <hal/rv64/platform/device_tree.h>

#include <io/print.h>
#include <libutil/string.h>

#define LOG_SPACE "\e[4C"
#define LOG_SPACE_MULTIPLE "\e[%dC"
#define LOG_SPACE_WIDTH 4

static struct fdt_header *fdt_header_internal = 0;

static uint32_t to_little_32(const void *ptr)
{
    const uint8_t *p = (const uint8_t *)ptr;
    return ((uint32_t)(p[0] << 24)) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8)
         | ((uint32_t)p[3]);
}

static uint64_t to_little_64(const void *ptr)
{
    const uint8_t *p = ptr;
    return ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40)
         | ((uint64_t)p[3] << 32) | ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16)
         | ((uint64_t)p[6] << 8) | ((uint64_t)p[7]);
}

static void parse_dtb_reserved_memory(struct fdt_header *);
static void parse_dtb_free_memory(struct fdt_header *);

void parse_dtb(uintptr_t fdt_base)
{
    HAL_LOG("fdt_base: 0x%x", fdt_base);
    if (!fdt_base)
    {
        ERROR_LOG("Device Tree Base Address is NULL");
        return;
    }

    struct fdt_header *header = (struct fdt_header *)fdt_base;

    if (to_little_32(&header->magic) != DTB_MAGIC)
    {
        ERROR_LOG("Invalid Device Tree Blob magic number: 0x%X", to_little_32(&header->magic));
        return;
    }

    HAL_LOG("Device Tree Header:");
    HAL_LOG(LOG_SPACE "Device Tree Base Address: 0x%x", fdt_base);
    HAL_LOG(LOG_SPACE "Magic: 0x%X", to_little_32(&header->magic));
    HAL_LOG(LOG_SPACE "Version: %u.%u", header->version >> 16, header->version & 0xFFFF);
    HAL_LOG(LOG_SPACE "Total Size: %u bytes", to_little_32(&header->totalsize));
    HAL_LOG(LOG_SPACE "Off Mem Rsv: %u bytes", to_little_32(&header->off_mem_rsvmap));
    HAL_LOG(LOG_SPACE "Off Dt Struct: %u bytes", to_little_32(&header->off_dt_struct));
    HAL_LOG(LOG_SPACE "Off Dt Strings: %u bytes", to_little_32(&header->off_dt_strings));
    HAL_LOG(LOG_SPACE "Version: %u.%u", header->version >> 16, header->version & 0xFFFF);
    HAL_LOG(
        LOG_SPACE "Last Comp Version: %u.%u",
        header->last_comp_version >> 16,
        header->last_comp_version & 0xFFFF
    );
    HAL_LOG(LOG_SPACE "Boot CPU ID: %u", to_little_32(&header->boot_cpuid_phys));
    HAL_LOG(LOG_SPACE "Size of Struct: %u bytes", to_little_32(&header->size_dt_struct));

    fdt_header_internal = header;

    parse_dtb_reserved_memory(header);
    parse_dtb_free_memory(header);
}

static void parse_dtb_reserved_memory(struct fdt_header *header)
{
    HAL_LOG("Parsing Reserved Memory Regions...");

    uint32_t       offset  = to_little_32(&header->off_mem_rsvmap);
    const uint8_t *current = (const uint8_t *)header + offset;

    for (;;)
    {
        uint64_t address  = to_little_64(current);
        uint64_t size     = to_little_64(current + sizeof(uint64_t));
        current          += (sizeof(uint64_t) * 2);

        if (address == 0 && size == 0)
        {
            // End of reserved memory entries
            break;
        }

        HAL_LOG("Reserved Memory Region: Address = 0x%x, Size = %u bytes", address, size);
    }
}

static void parse_dtb_free_memory(struct fdt_header *header)
{
    HAL_LOG("Parsing Free Memory Regions...");

    uint32_t       struct_offset = to_little_32(&header->off_dt_struct);
    uint32_t       string_offset = to_little_32(&header->off_dt_strings);
    const uint8_t *struct_blk    = (const uint8_t *)header + struct_offset;
    const uint8_t *string_blk    = (const uint8_t *)header + string_offset;
    const uint8_t *current       = struct_blk;
    word           remain        = to_little_32(&header->size_dt_struct);
    int            depth         = 0; // node depth
    const char    *path_stack[32];

    word offset = current - struct_blk;

    while (remain >= sizeof(uint32_t))
    {
        uint32_t token  = to_little_32(current);
        current        += sizeof(uint32_t);
        remain         -= sizeof(uint32_t);

        switch (token)
        {
            case FDT_BEGIN_NODE :
                {
                    HAL_LOG(
                        LOG_SPACE_MULTIPLE "FDT_BEGIN_NODE at offset 0x%X",
                        depth * LOG_SPACE_WIDTH,
                        struct_offset - remain
                    );
                    // skip node name
                    path_stack[depth++] = (const char *)current;

                    while (*current != '\0')
                    {
                        current++;
                        remain--;
                    }

                    // skip null terminator
                    current++;
                    remain--;

                    // align to 4-byte boundary
                    while ((word)current & 0b11)
                    {
                        current++;
                        remain--;
                    }

                    break;
                }

            case FDT_END_NODE :
                {
                    /*
                    HAL_LOG(
                        LOG_SPACE_MULTIPLE "FDT_END_NODE at offset 0x%X",
                        depth * LOG_SPACE_WIDTH,
                        struct_offset - remain
                    );
                    */
                    depth--;
                    break;
                }

            case FDT_PROP :
                {
                    /*
                    HAL_LOG(
                        LOG_SPACE_MULTIPLE "FDT_PROP at offset 0x%X",
                        depth * LOG_SPACE_WIDTH,
                        struct_offset - remain
                    );
                    */
                    uint32_t len          = to_little_32(current);
                    uint32_t name_offset  = to_little_32(current + 4);
                    current              += (sizeof(uint32_t) * 2);
                    remain               -= (sizeof(uint32_t) * 2);

                    const char *name      = (const char *)(string_blk + name_offset);
                    if ((depth > 0 && strncmp(path_stack[depth - 1], "memory", 6) == 0)
                        && (strcmp(name, "reg") == 0))
                    {
                        word entries = len / (sizeof(uint64_t) * 2);
                        for (word i = 0; i < entries; i++)
                        {
                            uint64_t address = to_little_64(current + i * (sizeof(uint64_t) * 2));
                            uint64_t size    = to_little_64(current + i * (sizeof(uint64_t) * 3));

                            HAL_LOG(
                                LOG_SPACE_MULTIPLE
                                "Free Memory Region: Path = %s, Address = 0x%x, Size = "
                                "0x%x bytes",
                                depth * LOG_SPACE_WIDTH,
                                path_stack[depth - 1],
                                address,
                                size
                            );
                        }
                    }

                    HAL_LOG(
                        LOG_SPACE_MULTIPLE COLOR_GREEN "%s" COLOR_RESET " (%u bytes)",
                        depth * LOG_SPACE_WIDTH,
                        name,
                        len
                    );

                    // align to 4-byte boundary and skip the property data
                    word skip  = (len + 3) & ~0b11;
                    current   += skip;
                    remain    -= skip;
                    break;
                }

            case FDT_NOP :
                HAL_LOG(
                    LOG_SPACE_MULTIPLE "FDT_NOP at offset 0x%X",
                    depth * LOG_SPACE_WIDTH,
                    struct_offset - remain
                );
                break;

            case FDT_END :
                HAL_LOG(
                    LOG_SPACE_MULTIPLE "FDT_END at offset 0x%X",
                    depth * LOG_SPACE_WIDTH,
                    struct_offset - remain
                );
                return;

            default :
                ERROR_LOG(
                    LOG_SPACE_MULTIPLE "Unknown token 0x%X at offset 0x%X",
                    depth,
                    token,
                    struct_offset - remain
                );
                return;
        }
    }
}
