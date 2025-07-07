#ifndef OS_RTA_RV64_DEVICE_TREE_H
#define OS_RTA_RV64_DEVICE_TREE_H

#include <types.h>

// parse dtb

#define DTB_MAGIC 0xd00dfeed

struct fdt_header
{
    uint32_t magic;             // 0xd00dfeed
    uint32_t totalsize;         // total size of the device tree blob
    uint32_t off_dt_struct;     // offset to the start of the device tree structure
    uint32_t off_dt_strings;    // offset to the start of the strings
    uint32_t off_mem_rsvmap;    // offset to the memory reservation map
    uint32_t version;           // version of the device tree format
    uint32_t last_comp_version; // last compatible version of the device tree format
    uint32_t boot_cpuid_phys;   // physical CPU ID of the boot CPU
    uint32_t size_dt_strings;   // size of the strings section
    uint32_t size_dt_struct;    // size of the device tree structure
} __attribute__((packed));

struct reserved_memory
{
    uint64_t address; // start address of the reserved memory region
    uint64_t size;    // size of the reserved memory region
} __attribute__((packed));

#define FDT_BEGIN_NODE 0x00000001U
#define FDT_END_NODE 0x00000002U
#define FDT_PROP 0x00000003U
#define FDT_NOP 0x00000004U
#define FDT_END 0x00000009U

void parse_dtb(uintptr_t fdt_base);

#endif
