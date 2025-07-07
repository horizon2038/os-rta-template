#ifndef OS_RTA_MEMORY_ALLOCATOR_H
#define OS_RTA_MEMORY_ALLOCATOR_H

#include <types.h>

// 2-word
struct memory_zone
{
    // align
    alignas(sizeof(word)) word size;
    struct memory_zone *next;
};

void  os_rta_kallocator_init(struct memory_zone *zone);
void *kmalloc(uintmax_t size);
void  kfree(void *ptr);

#endif
