SRC_DIR := ./src
BUILD_DIR := ./build
FONTS_DIR := ./fonts
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
C_FILES := $(wildcard $(SRC_DIR)/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
FONT_FILES := $(wildcard $(FONTS_DIR)/*.psf)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES)) \
        $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES)) \
		$(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_FILES)) \
		$(patsubst $(FONTS_DIR)/%.psf,$(BUILD_DIR)/%.o,$(FONT_FILES)) \

CFLAGS := -ffreestanding -m32 -g -c -mgeneral-regs-only \
	      -Wall -O0 -mno-red-zone -I ./include


CYAN := "\033[0;36m"
NOCOLOR := "\033[0m"

all: bin/OS.bin 

# Finished OS Binary
bin/OS.bin : $(BUILD_DIR)/full_kernel.bin $(SRC_DIR)/zeros.bin
	@cat $(BUILD_DIR)/full_kernel.bin $(SRC_DIR)/zeros.bin > bin/OS.bin
	@echo  $(CYAN) CAT $< $(NOCOLOR)


# # Kernel + Bootloader linking
# $(BUILD_DIR)/everything.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin
# 	@cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin > $(BUILD_DIR)/everything.bin
# 	@echo  $(CYAN) CAT $< $(NOCOLOR)


# Link all objects to make Kernel (without some of bootloader)
$(BUILD_DIR)/full_kernel.bin: $(OBJS)
	@/usr/local/i386elfgcc/bin/i386-elf-ld -T linker.ld $^
	@echo  $(CYAN) LD $< $(NOCOLOR)

# Compile All CPP files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@/usr/local/i386elfgcc/bin/i386-elf-g++ $(CFLAGS) -o $@ ./$<
	@echo  $(CYAN) CC $< $(NOCOLOR)

# Compile All C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@/usr/local/i386elfgcc/bin/i386-elf-gcc $(CFLAGS) -o $@ ./$<
	@echo  $(CYAN) CC $< $(NOCOLOR)

# Compile All ASM files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@nasm -f elf -o $@ ./$<
	@echo  $(CYAN) ASM $< $(NOCOLOR)

# "Compile" All Fonts files
$(BUILD_DIR)/%.o: $(FONTS_DIR)/%.psf
	@objcopy -O elf32-i386 -B i386 -I binary ./fonts/Uni2-Terminus12x6.psf ./build/Uni2-Terminus12x6.o
	@echo  $(CYAN) OBJCOPY $< $(NOCOLOR)

run:
	@qemu-system-i386 -drive format=raw,file="./bin/OS.bin",index=0,if=floppy,  -m 128M -gdb tcp::1234 -serial telnet:localhost:4321,server,nowait &

debug:
	@echo $(OBJS)
clean:
	@rm -rf $(BUILD_DIR)/* ./bin/*


	 