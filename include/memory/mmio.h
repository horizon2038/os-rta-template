#ifndef OS_RTA_MEMORY_MMIO_H
#define OS_RTA_MEMORY_MMIO_H

#include <types.h>

#define MMIO_READ_REGISTER(address) (*(volatile word *)(address))
#define MMIO_WRITE_REGISTER(address, value) (*(volatile word *)(address) = (value))

#endif
