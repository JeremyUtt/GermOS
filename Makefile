SRC_DIR := src
BUILD_DIR := build
FONTS_DIR := fonts
CC_DIR := /opt/i386elfgcc/bin
NASM := /usr/bin/nasm
INCLUDE_DIR := include
CFLAGS := -ffreestanding -m32 -g -c -mgeneral-regs-only \
	      -Wall -Werror -O0 -mno-red-zone -I ./include 

# Color codes for terminal/STDOUT text coloring
# CYAN := "\033[0;36m"
# NOCOLOR := "\033[0m"

# Get list of all types of files 
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
C_FILES := $(wildcard $(SRC_DIR)/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
FONT_FILES := $(wildcard $(FONTS_DIR)/*.psf)

# Combine all above lists and convert all file extensions to ".o"
OBJS := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_FILES)) \
		$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES)) \
        $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES)) \
		$(patsubst $(FONTS_DIR)/%.psf,$(BUILD_DIR)/%.o,$(FONT_FILES)) 



all: bin/OS.bin bin/OS.sym


# ============================================================
# ===== Start by compiling all source files into Objects =====
# ============================================================

# Compile all CPP files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@$(CC_DIR)/i386-elf-g++ $(CFLAGS) -o $@ ./$<
# @echo  $(CYAN) CC $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0CC $< \033[0m\n"

# Compile all C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC_DIR)/i386-elf-gcc $(CFLAGS) -o $@ ./$<
# @echo  $(CYAN) CC $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0CC $< \033[0m\n"
 

# Assemble all ASM files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@ # nasm -f elf -o $@ ./$<
	@$(NASM) src/kernel_entry.asm -f elf -o build/kernel_entry.o
# @echo $(CYAN) ASM $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0ASM $< \033[0m\n"

# "Compile" all Fonts files (convert to binart blob object)
$(BUILD_DIR)/%.o: $(FONTS_DIR)/%.psf
	@objcopy -O elf32-i386 -B i386 -I binary $< $@
# @echo  $(CYAN) OBJCOPY $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"


# ============================================================
# ===== Secondary Linking process, start combining files together
# ============================================================

# binary file for part of bootloader
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot.S
	@nasm $(SRC_DIR)/boot.S -f bin -o $(BUILD_DIR)/boot.bin
# @echo  $(CYAN) ASM $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0ASM $< \033[0m\n"


# Link all objects (exept above entry)to make Kernel 
$(BUILD_DIR)/Linking_Stage_1.bin: $(OBJS)
	@$(CC_DIR)/i386-elf-ld -o $@ -Ttext 0x1000  $^ --oformat binary
# @echo  $(CYAN) LD $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0LD$< \033[0m\n"


# Full Kernel (minus bootloader) Object file for Debugger 
bin/OS.sym: $(OBJS)
	@$(CC_DIR)/i386-elf-ld -Ttext 0x1000 -o $(BUILD_DIR)/Linking_Stage_1.o $^ 
	@objcopy --only-keep-debug $(BUILD_DIR)/Linking_Stage_1.o $@
# @echo  $(CYAN) OBJCOPY $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"


# Kernel + Bootloader linking
$(BUILD_DIR)/Linking_Stage_2.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/Linking_Stage_1.bin
	@cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/Linking_Stage_1.bin > $(BUILD_DIR)/Linking_Stage_2.bin
# @echo  $(CYAN) CAT $< $(NOCOLOR)#
	@printf "%b" "\033[0;36m\e0CAT $< \033[0m\n"

# Finished OS Binary
bin/OS.bin : $(BUILD_DIR)/Linking_Stage_2.bin $(SRC_DIR)/zeros.bin
	@cat $(BUILD_DIR)/Linking_Stage_2.bin $(SRC_DIR)/zeros.bin > bin/OS.bin
# @echo  $(CYAN) CAT $< $(NOCOLOR)
	@printf "%b" "\033[0;36m\e0CAT $< \033[0m\n"


run:
	@./run.sh
debug:
	@echo $(OBJS)
clean:
	@rm -rf $(BUILD_DIR)/* ./bin/*