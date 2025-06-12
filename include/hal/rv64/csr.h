#ifndef OS_RTA_RV64_CSR_H
#define OS_RTA_RV64_CSR_H

#include <types.h>

#define STRINGIFY(s) #s
#define STRINGIFY_HELPER(s) STRINGIFY(s)

#define OS_RTA_RV64_CSR_READ(csr) \
    ({ \
        unsigned long __tmp; \
        __asm__ volatile("csrr %0, " STRINGIFY_HELPER(csr) : "=r"(__tmp)); \
        __tmp; \
    })

#define OS_RTA_RV64_CSR_WRITE(csr, val) \
    ({ \
        unsigned long __tmp = (unsigned long)(val); \
        __asm__ volatile("csrw " STRINGIFY_HELPER(csr) ", %0" ::"r"(__tmp)); \
    })

// ===== SUPERVISOR MODE ===
// define supervisor-mode CSRs
#define CSR_SSTATUS 0x100
#define CSR_SIE 0x104
#define CSR_STVEC 0x105
#define CSR_SCOUNTEREN
#define CSR_SSCRATCH 0x140
#define CSR_SEPC 0x141
#define CSR_SCAUSE 0x142
#define CSR_STVAL 0x143
#define CSR_SIP 0x144

// define sstatus bits
#define SSTATUS_SIE (1 << 1) // Supervisor Interrupt Enable

// define sie bits
#define SIE_SSIE (1 << 1) // Supervisor Software Interrupt Enable
#define SIE_STIE (1 << 5) // Supervisor Timer Interrupt Enable
#define SIE_SEIE (1 << 9) // Supervisor External Interrupt Enable

// ===== MACHINE MODE ===
// define machine-mode CSRs
#define CSR_MSTATUS 0x300
#define CSR_MISA 0x301
#define CSR_MEDELEG 0x302
#define CSR_MIDELEG 0x303
#define CSR_MIE 0x304
#define CSR_MTVEC 0x305
#define CSR_MSCRATCH 0x340
#define CSR_MEPC 0x341
#define CSR_MCAUSE 0x342
#define CSR_MTVAL 0x343
#define CSR_MIP 0x344
#define CSR_MVENDORID 0xF11
#define CSR_MARCHID 0xF12
#define CSR_MIMPID 0xF13
#define CSR_MHARTID 0xF14

// define mstatus bits (reserved)
#define MSTATUS_UIE (1 << 0)  // User Interrupt Enable
#define MSTATUS_SIE (1 << 1)  // Supervisor Interrupt Enable
#define MSTATUS_MIE (1 << 3)  // Machine Interrupt Enable
#define MSTATUS_UPIE (1 << 4) // User Previous Interrupt Enable
#define MSTATUS_SPIE (1 << 5) // Supervisor Previous Interrupt Enable
#define MSTATUS_MPIE (1 << 7) // Machine Previous Interrupt Enable
#define MSTATUS_SPP (1 << 8)  // Supervisor Previous Privilege Mode
#define MSTATUS_MPP (1 << 11)

#endif
