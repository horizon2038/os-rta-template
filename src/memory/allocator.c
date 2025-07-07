#include <io/print.h>
#include <memory/allocator.h>
#include <types.h>

static struct memory_zone *memory_zone_head;

// k & r malloc

void os_rta_kallocator_init(struct memory_zone *zone)
{
    if (!zone)
    {
        ERROR_LOG("initial memory allocator failed: zone is NULL");
    }
}

void *kmalloc(uintmax_t size)
{
    if (!memory_zone_head)
    {
        ERROR_LOG("memory allocator not initialized");
        return 0;
    }

    for (struct memory_zone *current_zone = memory_zone_head; current_zone->next;
         current_zone                     = current_zone->next)
    {
        if (current_zone->size < size)
        {
            continue;
        }

        // Allocate memory from the current zone
        void *allocated_memory = (void *)((uint8_t *)(current_zone + sizeof(struct memory_zone))
        );                          // Assuming memory follows the zone
                                    // structure
        current_zone->size -= size; // Decrease the size of the zone
    }

    return 0;
}

void kfree(void *ptr)
{
    if (!ptr)
    {
        ERROR_LOG("kfree called with NULL pointer");
        return;
    }

    // Free the memory pointed to by ptr
}
