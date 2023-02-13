SRC_DIR := ./src
BUILD_DIR := ./build
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
C_FILES := $(wildcard $(SRC_DIR)/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES)) \
        $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES)) \
		$(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_FILES))

CFLAGS := -ffreestanding -m32 -g -c -mgeneral-regs-only \
	      -Wreturn-local-addr -mno-red-zone -I ./include

all: bin/OS.bin

# Finished OS Binary
bin/OS.bin : $(BUILD_DIR)/everything.bin $(SRC_DIR)/zeros.bin
	@cat $(BUILD_DIR)/everything.bin $(SRC_DIR)/zeros.bin > bin/OS.bin

# Kernel + Bootloader linking
$(BUILD_DIR)/everything.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin
	@cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin > $(BUILD_DIR)/everything.bin

# binary file for part of bootloader
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot.S
	@nasm $(SRC_DIR)/boot.S -f bin -o $(BUILD_DIR)/boot.bin

# Link all objects to make Kernel (without some of bootloader)
$(BUILD_DIR)/full_kernel.bin: $(OBJS)
	@i386-elf-ld -Ttext 0x1000 --oformat binary -o $@ $^ 


# Compile All CPP files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@i386-elf-gcc $(CFLAGS) -o $@ $<

# Compile All C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@i386-elf-gcc $(CFLAGS) -o $@ $<

# Compile All ASM files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@nasm -f elf -o $@ $<

run:
	@qemu-system-i386 -drive format=raw,file="./bin/OS.bin",index=0,if=floppy,  -m 128M -gdb tcp::1234 -serial telnet:localhost:4321,server,nowait &

debug:
	@echo $(OBJS)
clean:
	@rm -rf $(BUILD_DIR)/* ./bin/*


	 