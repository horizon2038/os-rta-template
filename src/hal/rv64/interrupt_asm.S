.section .text
.global __os_rta_rv64_handle_interrupt

.align 4
__os_rta_rv64_handle_interrupt:
    addi sp, sp, -256

    sd ra, 8 * 0(sp)    # x1
    sd gp, 8 * 2(sp)    # x3
    sd tp, 8 * 3(sp)    # x4
    sd t0, 8 * 4(sp)    # x5
    sd t1, 8 * 5(sp)    # x6
    sd t2, 8 * 6(sp)    # x7
    sd s0, 8 * 7(sp)    # x8
    sd s1, 8 * 8(sp)    # x9
    sd a0, 8 * 9(sp)    # x10
    sd a1, 8 * 10(sp)   # x11
    sd a2, 8 * 11(sp)   # x12
    sd a3, 8 * 12(sp)   # x13
    sd a4, 8 * 13(sp)   # x14
    sd a5, 8 * 14(sp)   # x15
    sd a6, 8 * 15(sp)   # x16
    sd a7, 8 * 16(sp)   # x17
    sd s2, 8 * 17(sp)   # x18
    sd s3, 8 * 18(sp)   # x19
    sd s4, 8 * 19(sp)   # x20
    sd s5, 8 * 20(sp)   # x21
    sd s6, 8 * 21(sp)   # x22
    sd s7, 8 * 22(sp)   # x23
    sd s8, 8 * 23(sp)   # x24
    sd s9, 8 * 24(sp)   # x25
    sd s10, 8 * 25(sp)  # x26
    sd s11, 8 * 26(sp)  # x27
    sd t3, 8 * 27(sp)   # x28
    sd t4, 8 * 28(sp)   # x29
    sd t5, 8 * 29(sp)   # x30
    sd t6, 8 * 30(sp)   # x31

    csrw sepc, a0

    ld ra, 8 * 0(sp)
    ld gp, 8 * 2(sp)
    ld tp, 8 * 3(sp)
    ld t0, 8 * 4(sp)
    ld t1, 8 * 5(sp)
    ld t2, 8 * 6(sp)
    ld s0, 8 * 7(sp)
    ld s1, 8 * 8(sp)
    ld a0, 8 * 9(sp)   
    ld a1, 8 * 10(sp)
    ld a2, 8 * 11(sp)
    ld a3, 8 * 12(sp)
    ld a4, 8 * 13(sp)
    ld a5, 8 * 14(sp)
    ld a6, 8 * 15(sp)
    ld a7, 8 * 16(sp)
    ld s2, 8 * 17(sp)
    ld s3, 8 * 18(sp)
    ld s4, 8 * 19(sp)
    ld s5, 8 * 20(sp)
    ld s6, 8 * 21(sp)
    ld s7, 8 * 22(sp)
    ld s8, 8 * 23(sp)
    ld s9, 8 * 24(sp)
    ld s10, 8 * 25(sp)
    ld s11, 8 * 26(sp)
    ld t3, 8 * 27(sp)
    ld t4, 8 * 28(sp)
    ld t5, 8 * 29(sp)
    ld t6, 8 * 30(sp)

    addi sp, sp, 256

    sret
