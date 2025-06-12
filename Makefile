# Target executable name
TARGET = kernel.elf

# LLVM/Clang toolchain
CC     = clang
# Use clang as the linker driver, which will call lld
LD     = clang

# C compiler flags
# --target is crucial for cross-compiling
CFLAGS  = --target=riscv64-unknown-elf -Wall -Wextra -std=c11 -ffreestanding -nostdlib -O0 -ggdb -mcmodel=medany -Iinclude

# Linker flags
# Pass linker-specific flags using -Wl,
LDFLAGS = -T linker.ld -nostdlib -Wl,--gc-sections -Wl,-Map,build/kernel.map

# Source files: Recursively find all .c and .S files in the src directory
C_SRC  := $(shell find src -name '*.c')
AS_SRC := $(shell find src -name '*.S')

# Object files: Maintain the directory structure from src in the build directory
OBJ_C  := $(patsubst src/%.c,build/%.o,$(C_SRC))
OBJ_AS := $(patsubst src/%.S,build/%.o,$(AS_SRC))
OBJS   := $(OBJ_C) $(OBJ_AS)

# Dependency files for header changes
DEPS   := $(OBJS:.o=.d)

# QEMU settings
QEMU       ?= qemu-system-riscv64
QEMU_FLAGS ?= -machine virt -nographic -bios default -smp 1 -m 128M -serial mon:stdio --no-shutdown --no-reboot

# Phony targets
.PHONY: all clean run debug

# Default target
all: build/$(TARGET)

# Include generated dependency files
-include $(DEPS)

# Linking the final executable
# Use the compiler (clang) as the linker driver
build/$(TARGET): $(OBJS)
	$(LD) --target=riscv64-unknown-elf $(LDFLAGS) -o $@ $(OBJS)
	@echo " LD	$@"

# Static pattern rule for compiling C source files
# This handles any .c file from the src directory hierarchy
# and places its .o file in the corresponding build directory hierarchy.
$(OBJ_C): build/%.o: src/%.c
	@echo " CC	$<"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -MMD -MP $< -o $@

# Static pattern rule for assembling assembly source files
$(OBJ_AS): build/%.o: src/%.S
	@echo " AS	$<"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory (though the rules above also ensure subdirs exist)
build:
	@echo " MKDIR	build"
	@mkdir -p build

# Clean up build artifacts
clean:
	@echo " CLEAN"
	@rm -rf build

# Run the kernel in QEMU
run: build/$(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel build/$(TARGET)

# Run QEMU in debug mode (waiting for a debugger)
debug: build/$(TARGET)
	@echo "Debugging session started. Connect with LLDB using:"
	@echo "  lldb build/$(TARGET)"
	@echo "Then, in the LLDB shell, use 'gdb-remote 1234' to connect to QEMU."
	@echo "---"
	@echo "Alternatively, connect with GDB using:"
	@echo "  riscv64-unknown-elf-gdb build/$(TARGET)"
	@echo "  (gdb) target remote :1234"
	$(QEMU) $(QEMU_FLAGS) -kernel build/$(TARGET) -s -S

# Include a user-specific makefile if it exists
include user.mk

