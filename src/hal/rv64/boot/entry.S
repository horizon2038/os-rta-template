.section .text.entry
.globl _start

_start:
    la sp, kernel_stack_top
    .extern os_rta_rv64_c_entry
    call os_rta_rv64_c_entry

_hlt:
    wfi
    j _hlt

.section .bss.stack
.globl kernel_stack
.globl kernel_stack_top

.balign 4096
kernel_stack:
    .space 4096
kernel_stack_top:

