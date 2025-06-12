#include <hal/interrupt.h>
#include <hal/rv64/csr.h>
#include <hal/rv64/platform/plic.h>
#include <hal/timer.h>
#include <io/print.h>

static interrupt_handler   interrupt_handler_internal   = 0;
static kernel_call_handler kernel_call_handler_internal = 0;
static timer_handler       timer_handler_internal       = 0;

#define SUPERVISOR_SOFTWARE_INTERRUPT 1
#define SUPERVISOR_TIMER_INTERRUPT 5
#define SUPERVISOR_EXTERNAL_INTERRUPT 9

static void handle_timer_interrupt(void);
static void handle_software_interrupt(void);
static void handle_external_interrupt(void);

void os_rta_rv64_handle_interrupt(sword cause, sword epc)
{
    if (cause < 0)
    {
        sword cause_code = cause & ~(1ULL << ((sizeof(word) * 8) - 1));
        DEBUG_LOG("cause code: 0x%x", cause_code);

        switch (cause_code & 0xFF)
        {
            case SUPERVISOR_SOFTWARE_INTERRUPT :
                handle_software_interrupt();
                return;

            case SUPERVISOR_TIMER_INTERRUPT :
                handle_timer_interrupt();
                return;

            case SUPERVISOR_EXTERNAL_INTERRUPT :
                handle_external_interrupt();
                return;

            default :
                ERROR_LOG("Unknown interrupt cause: 0x%x", cause_code);
                return;
        }
    }

    ERROR_LOG("Exception occured with cause: 0x%x at EPC: 0x%x", cause, epc);
}

static void handle_software_interrupt(void)
{
    kernel_call_handler_internal(0x777);
}

static void handle_timer_interrupt(void)
{
    if (!timer_handler_internal)
    {
        ERROR_LOG("No timer handler registered");
        return;
    }

    DEBUG_LOG("Handling timer interrupt");
    timer_handler_internal();
}

static void handle_external_interrupt(void)
{
    sword irq_number = os_rta_rv64_plic_read_claim();
    DEBUG_LOG("Handling external interrupt with IRQ number: %d", irq_number);

    if (!interrupt_handler_internal)
    {
        ERROR_LOG("No external interrupt handler registered for IRQ %d", irq_number);
    }

    interrupt_handler_internal(irq_number);
    os_rta_rv64_plic_write_claim(irq_number);
}

static void supervisor_configure_interrupt_handler(void);
static void supervisor_interrupt_init(void);
void        __os_rta_rv64_handle_interrupt(void);

void os_rta_hal_interrupt_init(void)
{
    HAL_LOG("Initializing interrupt system...");
    supervisor_configure_interrupt_handler();
    os_rta_rv64_plic_init();
    supervisor_interrupt_init();
}

void os_rta_hal_interrupt_register_handler(interrupt_handler handler)
{
    HAL_LOG("Registering interrupt handler...");
    if (handler == 0)
    {
        ERROR_LOG("Interrupt handler is NULL");
        return;
    }

    interrupt_handler_internal = handler;
}

static void supervisor_configure_interrupt_handler(void)
{
    HAL_LOG("Configuring supervisor interrupt handler...");

    OS_RTA_RV64_CSR_WRITE(CSR_STVEC, &__os_rta_rv64_handle_interrupt);

    HAL_LOG("Supervisor interrupt handler configured.");
}

static void supervisor_interrupt_init(void)
{
    HAL_LOG("Setting up machine-level interrupts...");

    // NOTE: (supervisor-mode)
    // - stvec: store the address of the trap handler
    // - stval: store the value that caused the trap
    // - sepc: store the program counter at the time of the trap
    // - sstatus: store the mode that the CPU was in when the trap occurred

    word sie_bits = SIE_STIE | SIE_SEIE | SIE_SSIE;
    OS_RTA_RV64_CSR_WRITE(CSR_SIE, sie_bits);
    OS_RTA_RV64_CSR_WRITE(CSR_SSTATUS, OS_RTA_RV64_CSR_READ(CSR_SSTATUS) | SSTATUS_SIE);

    HAL_LOG("Supervisor interrupts enabled.");
}

void os_rta_hal_kernel_call_init(void)
{
    HAL_LOG("Initializing kernel call handler...");
    // do nothing for now
}

void os_rta_hal_kernel_call_register_handler(kernel_call_handler handler)
{
    if (handler == 0)
    {
        ERROR_LOG("Kernel call handler is NULL");
        return;
    }

    kernel_call_handler_internal = handler;

    HAL_LOG("Kernel call handler registered successfully.");
}

void os_rta_hal_timer_init(timer_handler target_handler)
{
    if (target_handler == 0)
    {
        ERROR_LOG("Timer handler is NULL");
        return;
    }

    timer_handler_internal = target_handler;

    // In a real implementation, you would set up the hardware timer here.
    // For this example, we will just simulate a timer event.
    HAL_LOG("Timer registered successfully");
}
