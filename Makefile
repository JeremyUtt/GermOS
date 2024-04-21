# ===================================================
# ===== Setup Initial Parameters and File Paths =====
# ===================================================

# Directories
SRC_DIR := src
BUILD_DIR := build
FONTS_DIR := fonts
INCLUDE_DIR := include
CC_DIR := /opt/i386elfgcc/bin

# Programs
NASM := /usr/bin/nasm

# Flags
CFLAGS := -ffreestanding -m32 -g -c -mgeneral-regs-only \
	      -Wall -Werror -O3 -mno-red-zone -I ./$(INCLUDE_DIR)
LDFLAGS := -T linker.ld --no-warn-rwx-segments

# ===============================================
# ===== Setup Files and Variables for later =====
# ===============================================

MODEFLAGS := -D TEXT_MODE
# Color codes for terminal/STDOUT text coloring
# CYAN := "\033[0;36m"
# NOCOLOR := "\033[0m"

# Get list of all types of files 
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
C_FILES := $(wildcard $(SRC_DIR)/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
FONT_FILES := $(wildcard $(FONTS_DIR)/*.psf)
IMAGE_FILES := $(wildcard $(FONTS_DIR)/*.ppm)

# Combine all above lists and convert all file extensions to ".o"
OBJS := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_FILES)) \
		$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES)) \
        $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES)) \
		$(patsubst $(FONTS_DIR)/%.psf,$(BUILD_DIR)/%.o,$(FONT_FILES)) \
		$(patsubst $(FONTS_DIR)/%.ppm,$(BUILD_DIR)/%.o,$(IMAGE_FILES))

all: bin/OS.bin bin/OS.sym

gui:
	@make MODEFLAGS="" --no-print-directory

# ============================================================
# ===== Start by compiling all source files into Objects =====
# ============================================================

# Compile all CPP files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@$(CC_DIR)/i386-elf-g++ $(CFLAGS) $(MODEFLAGS) -o $@ ./$<
	@printf "%b" "\033[0;36m\e0CC $< \033[0m\n"

# Compile all C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC_DIR)/i386-elf-gcc $(CFLAGS) $(MODEFLAGS) -o $@ ./$<
	@printf "%b" "\033[0;36m\e0CC $< \033[0m\n"
 
# Assemble all ASM files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@$(NASM) -g $(MODEFLAGS) -f elf -o $@ ./$< -w+gnu-elf-extensions
	@printf "%b" "\033[0;36m\e0ASM $< \033[0m\n"

# "Compile" all Fonts files (convert to binary blob object)
$(BUILD_DIR)/%.o: $(FONTS_DIR)/%.psf
	@objcopy -O elf32-i386 -B i386 -I binary $< $@
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"


# "Compile" all ppm files (convert to binary blob object)
$(BUILD_DIR)/%.o: $(FONTS_DIR)/%.ppm
	@objcopy -O elf32-i386 -B i386 -I binary $< $@
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"

# =====================================================================
# ===== Secondary Linking process, start combining files together =====
# =====================================================================

# Link All Object Files To ELF executable
$(BUILD_DIR)/OS.o : $(OBJS)
	@$(CC_DIR)/i386-elf-ld -o $@ $^ $(LDFLAGS)
	@printf "%b" "\033[0;36m\e0LD $< \033[0m\n"

# Convert ELF file to pure binary (OS.bin)
bin/OS.bin : $(BUILD_DIR)/OS.o $(SRC_DIR)/zeros.bin
	@objcopy -O binary $(BUILD_DIR)/OS.o $(BUILD_DIR)/OS1.bin
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"
	@cat $(BUILD_DIR)/OS1.bin $(SRC_DIR)/zeros.bin > $@
	@printf "%b" "\033[0;36m\e0CAT $< \033[0m\n"

# Convert ELF file to Symbol file for Debugger (OS.sym) 
bin/OS.sym: $(BUILD_DIR)/OS.o
	@objcopy --only-keep-debug $(BUILD_DIR)/OS.o $@
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"

# =====================================
# ===== Define othe make commands =====
# =====================================

run: all
	@./run.sh
debug:
	@echo $(OBJS)
clean:
	@rm -rf $(BUILD_DIR)/* ./bin/*
format:
	@find $(SRC_DIR) $(INCLUDE_DIR) -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i