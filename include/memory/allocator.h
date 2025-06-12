#ifndef OS_RTA_MEMORY_ALLOCATOR_H
#define OS_RTA_MEMORY_ALLOCATOR_H

#include <stdint.h>

void *kmalloc(uintmax_t size);
void  kfree(void *ptr);

#endif
