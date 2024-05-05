# ===================================================
# ===== Setup Initial Parameters and File Paths =====
# ===================================================

# Directories
SRC_DIR := src
BUILD_DIR := build
BLOBS_DIR := files
INCLUDE_DIR := include
CC_DIR := /opt/i386elfgcc/bin

# Programs
NASM := /usr/bin/nasm

# Flags
CFLAGS := -ffreestanding -m32 -g -c -mgeneral-regs-only \
	      -Wall -Werror -mno-red-zone -I ./$(INCLUDE_DIR) \
		  -fno-exceptions
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
BLOBS := $(wildcard $(BLOBS_DIR)/*)

# Combine all above lists and convert all file extensions to ".o"
OBJS := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_FILES)) \
		$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_FILES)) \
        $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES)) \
		$(patsubst $(BLOBS_DIR)/%,$(BUILD_DIR)/%.o,$(BLOBS)) 

all: bin/OS.sym bin/OS.bin info

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
	@$(NASM) -g $(MODEFLAGS) -f elf -o $@ ./$< -w+gnu-elf-extensions -l $@listing.txt
	@printf "%b" "\033[0;36m\e0ASM $< \033[0m\n"

# "Compile" all Blobs (convert to binary blob object). Second line renames start symbol to be just the filename without puncuation
$(BUILD_DIR)/%.o: $(BLOBS_DIR)/%
	@objcopy -O elf32-i386 -B i386 -I binary $< $@ 
	@objcopy --redefine-sym $$(echo -n _binary_$< | tr -c '[A-Za-z0-9]' '_')_start=$$(echo -n $< | sed 's/.*\///' | tr -cd '[A-Za-z0-9]') $@
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

# i <3 Copilot and Generative AI!
info:
	@echo "INFO: Bootloader uses \
	$$(printf "%d" 0x$$(grep "times 510-" build/boot.olisting.txt | awk '{print $$2}')) \
	bytes (\
	$$(echo "scale=2; (($$(printf "%d" 0x$$(grep "times 510-" build/boot.olisting.txt | awk '{print $$2}')) / 512) * 100)" | bc)\
	%) of the boot sector" 

	@echo "INFO: Kernel uses $$(du -k bin/OS.bin | cut -f1) KB of memory"

# Convert ELF file to Symbol file for Debugger (OS.sym) 
bin/OS.sym: $(BUILD_DIR)/OS.o
	@objcopy --only-keep-debug $(BUILD_DIR)/OS.o $@
	@printf "%b" "\033[0;36m\e0OBJCOPY $< \033[0m\n"

# =====================================
# ===== Define othe make commands =====
# =====================================

run: bin/OS.sym bin/OS.bin
	@./run.sh
debug: bin/OS.sym bin/OS.bin
	@./run.sh debug

converter:
	@g++ imageConverter/convertToGimg.cpp -o imageConverter/gimgConvert
	@echo Done

clean:
	@rm -rf $(BUILD_DIR)/* ./bin/* ./imageConverter/gimgConvert
format:
	@find $(SRC_DIR) $(INCLUDE_DIR) -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i