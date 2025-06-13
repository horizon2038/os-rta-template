#ifndef OS_RTA_HAL_PRINT_H
#define OS_RTA_HAL_PRINT_H

#include <hal/put_char.h>

#define COLOR_GREEN "\e[32m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_RED "\e[31m"
#define COLOR_BLUE "\e[34m"
#define COLOR_RESET "\e[0m"

#define LOG_KERNEL COLOR_GREEN "[KERNEL] " COLOR_RESET

#define LOG_ALIGN "\e[10G"

#define KERNEL_LOG(fmt, ...) \
    do \
    { \
        printf(LOG_KERNEL LOG_ALIGN fmt "\r\n", ##__VA_ARGS__); \
    } while (0)

#define HAL_LOG(fmt, ...) \
    do \
    { \
        printf(COLOR_YELLOW "[   HAL] " COLOR_RESET LOG_ALIGN fmt "\r\n", ##__VA_ARGS__); \
    } while (0)

#define DEBUG_LOG(fmt, ...) \
    do \
    { \
        printf( \
            COLOR_BLUE "[ DEBUG] " COLOR_RESET LOG_ALIGN "%s@%s:%d: " fmt "\r\n", \
            __func__, \
            __FILE__, \
            __LINE__, \
            ##__VA_ARGS__ \
        ); \
    } while (0)

#define ERROR_LOG(fmt, ...) \
    do \
    { \
        printf( \
            COLOR_RED "[ ERROR] " COLOR_RESET LOG_ALIGN "%s@%s:%d: " fmt "\r\n", \
            __func__, \
            __FILE__, \
            __LINE__, \
            ##__VA_ARGS__ \
        ); \
        for (;;) \
            ; \
    } while (0)

void printf(const char *format, ...);

#endif
