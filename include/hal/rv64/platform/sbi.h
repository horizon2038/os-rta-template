#ifndef OS_RTA_RV64_SBI_H
#define OS_RTA_RV64_SBI_H

#include <types.h>

// sbi ret
struct sbi_ret
{
    // 0: success, 1: failed
    word error;
    // return value
    word value;
};

struct sbi_ret
    sbi_call(word eid, word fid, word arg0, word arg1, word arg2, word arg3, word arg4, word arg5);

struct sbi_ret sbi_set_timer(word time);
struct sbi_ret sbi_hart_start(word hart_id, word start_addr, word opaque);
struct sbi_ret sbi_hart_get_status(word hard_id);

// console
// struct sbi_ret sbi_console_putchar(word ch);
// struct sbi_ret sbi_console_getchar(void);

#endif
