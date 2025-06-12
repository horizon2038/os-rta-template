#include <hal/rv64/platform/plic.h>

#include <io/print.h>
#include <memory/mmio.h>

// WARN:
// when accessing PLIC registers, you must use a 32-bit access
#define MMIO_READ_REGISTER_32(address) (*(volatile uint32_t *)(address))
#define MMIO_WRITE_REGISTER_32(address, value) (*(volatile uint32_t *)(address) = (value))

static const word calculate_plic_priority_register(int irq)
{
    return PLIC_BASE + PLIC_PRIORITY + (irq * 4);
}

void os_rta_rv64_plic_init(void)
{
    HAL_LOG("Initializing PLIC...");

    // enable UART IRQ
    MMIO_WRITE_REGISTER_32(PLIC_BASE + PLIC_ENABLE, (0 << PLIC_UART_IRQ)); // UART
    MMIO_WRITE_REGISTER_32(calculate_plic_priority_register(PLIC_UART_IRQ), 1);

    // configure PLIC threshold
    MMIO_WRITE_REGISTER_32(PLIC_BASE + PLIC_THRESHOLD, 0);

    HAL_LOG("PLIC initialized successfully.");
}

sword os_rta_rv64_plic_read_claim(void)
{
    HAL_LOG("Reading PLIC claim...");

    // Read the claim register to get the IRQ number
    sword claim = MMIO_READ_REGISTER_32(PLIC_BASE + PLIC_CLAIM);
    if (claim == 0)
    {
        return -1; // No pending IRQs
    }

    return claim;
}

void os_rta_rv64_plic_write_claim(sword irq)
{
    HAL_LOG("Writing PLIC claim for IRQ %d...", irq);

    // Write the claim register to acknowledge the IRQ
    MMIO_WRITE_REGISTER_32(PLIC_BASE + PLIC_CLAIM, irq);
    HAL_LOG("Claim for IRQ %d acknowledged.", irq);
}
