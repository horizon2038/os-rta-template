#include <hal/rv64/platform/sbi.h>

#define SBI_EID_SET_TIMER 0x54494D5
#define SBI_EBI_HART_START 0x48534D
#define SBI_EBI_HART_GET_STATUS 0x735049

struct sbi_ret
    sbi_call(word eid, word fid, word arg0, word arg1, word arg2, word arg3, word arg4, word arg5)
{
    struct sbi_ret ret;

    register word a6 __asm__("a6") = fid;
    register word a7 __asm__("a7") = eid;

    register word a0 __asm__("a0") = arg0;
    register word a1 __asm__("a1") = arg1;
    register word a2 __asm__("a2") = arg2;
    register word a3 __asm__("a3") = arg3;
    register word a4 __asm__("a4") = arg4;
    register word a5 __asm__("a5") = arg5;

    __asm__ volatile(
        "ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
        : "memory"
    );

    ret.error = a0;
    ret.value = a1;

    return ret;
}

struct sbi_ret sbi_set_timer(word time)
{
    return sbi_call(SBI_EID_SET_TIMER, 0, time, 0, 0, 0, 0, 0);
}

struct sbi_ret sbi_hart_start(word hart_id, word start_addr, word opaque)
{
    return sbi_call(SBI_EBI_HART_START, 0, hart_id, start_addr, opaque, 0, 0, 0);
}

struct sbi_ret sbi_hart_get_status(word hart_id)
{
    return sbi_call(SBI_EBI_HART_GET_STATUS, 2, hart_id, 0, 0, 0, 0, 0);
}
