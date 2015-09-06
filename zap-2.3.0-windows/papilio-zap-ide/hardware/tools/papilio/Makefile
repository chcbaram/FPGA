# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# WinAVR Makefile Template written by Eric B. Weddington, Jörg Wunsch, et al.
#
# Released to the Public Domain
#
# Additional material for this makefile was written by:
# Peter Fleury
# Tim Henigan
# Colin O'Flynn
# Reiner Patommel
# Markus Pfaff
# Sander Pool
# Frederik Rouleau
# Carlos Lamas
#
#----------------------------------------------------------------------------
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF.
#
# make extcoff = Convert ELF to AVR Extended COFF.
#
# make program = Download the hex file to the device, using avrdude.
#                Please customize the avrdude settings below first!
#
# make debug = Start either simulavr or avarice as specified for debugging, 
#              with avr-gdb or avr-insight as the front end for debugging.
#
# make filename.s = Just compile filename.c into the assembler code only.
#
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------


# MCU name
MCU = atmega103


# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. You can then use this symbol in your source code to 
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
F_CPU = 16000000

# If PAPILIO_SIM is set to TRUE then the simulation memory init file at the designated path will be updated.
PAPILIO_SIM = FALSE
PAPILIO_SIM_PATH = "C:/Arduino-Soft-Core/sources/Memory/prog_mem_init.vhd"

# Output format. (can be srec, ihex, binary)
FORMAT = ihex


# Target file name (without extension).
TARGET = main


# Object files directory
#     To put object files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
OBJDIR = build


# List C source files here. (C dependencies are automatically generated.)
SRC = $(TARGET).c

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s


# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = dwarf-2

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99


# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL


# Place -D or -U options here for ASM sources
ADEFS = -DF_CPU=$(F_CPU)


# Place -D or -U options here for C++ sources
CPPDEFS = -DF_CPU=$(F_CPU)UL
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS



#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)


#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = -g$(DEBUG)
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
CPPFLAGS += -Wundef
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wa,-adhlns=$(<:%.cpp=$(OBJDIR)/%.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
#CPPFLAGS += $(CSTANDARD)


#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex 
#       dump that will be displayed for a given single line of source input.
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:%.S=$(OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100


#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

MATH_LIB = -lm

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref -nostartfiles
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
#LDFLAGS += -T linker_script.x

#---------------- Gadget Factory Soft Processor Options ----------------

# # Soft Processor bit file
# GF_SP_BIT = bin/top_avr_core_v8.bit

# # Soft Processor bmm file
# GF_SP_BMM = bin/top_avr_core_v8_bd.bmm


#============================================================================

#ifneq ($(findstring silent,$(MAKEFLAGS)),)

# SILENT=@

#endif

#Build directory
BUILDDIR = build\

# Define programs and commands.
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
WINSHELL = cmd
SREC_CAT = ./srec_cat
DATA2MEM = ./data2mem
GAWK = ./gawk
BUTTERFLY_LOADER = BF_Loader.bat
BUTTERFLY_PROG = ./papilio-prog


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_MEM = Converting Intel hex file to Verilog Mem format:
MSG_BIT = Merging Verilog Mem file with Xilinx bitstream:
MSG_DMP = Dumping contents of Xilinx bitstream so program code can be viewed:
MSG_BF_PROG = Loading Xilinx bitstream onto Butterfly Platform hardware:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:




# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 

# Define all listing files.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst) 


# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d


# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)





# Default target.
all: begin gccversion sizebefore build sizeafter end

# Change the build target to build a HEX file or a library.
build: elf hex eep lss sym mem bit dmp
#build: lib

papilio_papilioOne: hex mem p1bit bf_program
papilio_papilioOneBurn: hex mem p1bit bf_programBurn250K
papilio_papilioOne_500: hex mem pone500bit bf_program
papilio_papilioOne_500Burn: hex mem pone500bit bf_programBurn500K

papilio_papilioPlusLX4: hex mem pluslx4bit bf_program
apilio_papilioPlusLX4Burn: hex mem pluslx4bit bf_programBurn250K
papilio_papilioPlusLX9: hex mem pluslx9bit bf_program
papilio_papilioPlusLX9Burn: hex mem pluslx9bit bf_programBurnLX9

papilio_papilioLogicLX4: hex mem logiclx4bit bf_program
apilio_papilioLogicLX4Burn: hex mem logiclx4bit bf_programBurn250K
papilio_papilioLogicLX9: hex mem logiclx9bit bf_program
papilio_papilioLogicLX9Burn: hex mem logiclx9bit bf_programBurnLX9

papilio_custom: hex mem pcustom bf_program


elf: *.elf
hex: *.hex
	$(SILENT) @echo
	$(SILENT) @echo $(MSG_MEM)
	$(SREC_CAT) $< -Intel -Byte_Swap 2  -Data_Only -o tmp.mem -vmem 8
	$(GAWK) ' BEGIN{FS=" ";} { $$1= ""; print}' tmp.mem > out.mem
eep: *.eep
lss: *.lss
sym: *.sym
mem: *.mem
bit: *.bit
dmp: *.dmp
LIBNAME=lib$(TARGET).a
lib: $(LIBNAME)



# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo

p1bit:
	@echo
	@echo Selecting Papilio One Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/AVR8_PapilioOne_bd.bmm -bt bitstreams/AVR8_PapilioOne.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/AVR8_PapilioOne_bd.bmm -bt out.bit -d > out.dmp	
	
pone500bit:
	@echo
	@echo Selecting Papilio One 500K Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/AVR8_PapilioOne_500_bd.bmm -bt bitstreams/AVR8_PapilioOne_500.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/AVR8_PapilioOne_500_bd.bmm -bt out.bit -d > out.dmp		
	
pluslx4bit:
	@echo
	@echo Selecting Papilio Plus LX4 Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/AVR8_Vanilla_PapilioPlus_LX4_bd.bmm -bt bitstreams/AVR8_Vanilla_PapilioPlus_LX4.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/AVR8_Vanilla_PapilioPlus_LX4_bd.bmm -bt out.bit -d > out.dmp	

pluslx9bit:
	@echo
	@echo Selecting Papilio Plus LX9 Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/AVR8_Shifty_PapilioPlus_LX9_bd.bmm -bt bitstreams/AVR8_Shifty_PapilioPlus_LX9.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/AVR8_Shifty_PapilioPlus_LX9_bd.bmm -bt out.bit -d > out.dmp		
	
logiclx4bit:
	@echo
	@echo Selecting Papilio Plus LX4 Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/AVR8_Vanilla_PapilioLogic_LX4_bd.bmm -bt bitstreams/AVR8_Vanilla_PapilioLogic_LX4.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/AVR8_Vanilla_PapilioLogic_LX4_bd.bmm -bt out.bit -d > out.dmp	

logiclx9bit:
	@echo
	@echo Selecting Papilio Plus LX9 Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/AVR8_Shifty_PapilioLogic_LX9_bd.bmm -bt bitstreams/AVR8_Shifty_PapilioLogic_LX9.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/AVR8_Shifty_PapilioLogic_LX9_bd.bmm -bt out.bit -d > out.dmp		

pcustom:
	@echo
	@echo Selecting Papilio Custom Bit file.
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm bitstreams/custom_bd.bmm -bt bitstreams/custom.bit -bd out.mem -o b out.bit
	#$(DATA2MEM) -bm bitstreams/custom_bd.bmm -bt out.bit -d > out.dmp
ifeq ($(PAPILIO_SIM),TRUE)
	$(DATA2MEM) -bm bitstreams/custom_bd.bmm -bd out.mem -o h $(PAPILIO_SIM_PATH)
endif

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi



# Display compiler version information.
gccversion : 
	@$(CC) --version

# Program the device.  
program: *.hex *.mem
	@echo
	@echo Writing Bit file to the Hardware
	$(BUTTERFLY_LOADER) ..\..\out.bit

# Program the device.  
bf_program: *.hex *.mem
	@echo
	@echo Writing Bit file to the Hardware
	$(BUTTERFLY_PROG) -v -f out.bit -v
	
# Program the device to SPI Flash for 250K part.  
bf_programBurn250K: *.hex *.mem
	@echo
	@echo Burning Bit file to the SPI Flash
	@echo !!!!!!!!!!PLEASE BE PATIENT, BURNING TAKES A WHILE AND DOES NOT UPDATE IN REALTIME.!!!!!!!!!!
	$(BUTTERFLY_PROG) -v -f out.bit -b bscan_spi_xc3s250e.bit -sa
	$(BUTTERFLY_PROG) -v -f out.bit -v

# Program the device to SPI Flash for 500K part.  
bf_programBurn500K: *.hex *.mem
	@echo
	@echo Burning Bit file to the SPI Flash
	@echo !!!!!!!!!!PLEASE BE PATIENT, BURNING TAKES A WHILE AND DOES NOT UPDATE IN REALTIME.!!!!!!!!!!
	$(BUTTERFLY_PROG) -v -f out.bit -b bscan_spi_xc3s500e.bit -sa	
	$(BUTTERFLY_PROG) -v -f out.bit -v
	
# Program the device to SPI Flash for 500K part.  
bf_programBurnLX9: *.hex *.mem
	@echo
	@echo Burning Bit file to the SPI Flash
	@echo !!!!!!!!!!PLEASE BE PATIENT, BURNING TAKES A WHILE AND DOES NOT UPDATE IN REALTIME.!!!!!!!!!!
	$(BUTTERFLY_PROG) -v -f out.bit -b bscan_spi_lx9.bit -sa	
	$(BUTTERFLY_PROG) -v -f out.bit -v	
	

# Generate avr-gdb config/init file which does the following:
#     define the reset signal, load the target file, connect to target, and set 
#     a breakpoint at main().
gdb-config: 
	@$(REMOVE) $(GDBINIT_FILE)
	@echo define reset >> $(GDBINIT_FILE)
	@echo SIGNAL SIGHUP >> $(GDBINIT_FILE)
	@echo end >> $(GDBINIT_FILE)
	@echo file $(TARGET).elf >> $(GDBINIT_FILE)
	@echo target remote $(DEBUG_HOST):$(DEBUG_PORT)  >> $(GDBINIT_FILE)
ifeq ($(DEBUG_BACKEND),simulavr)
	@echo load  >> $(GDBINIT_FILE)
endif
	@echo break main >> $(GDBINIT_FILE)

debug: gdb-config $(TARGET).elf
ifeq ($(DEBUG_BACKEND), avarice)
	@echo Starting AVaRICE - Press enter when "waiting to connect" message displays.
	@$(WINSHELL) /c start avarice --jtag $(JTAG_DEV) --erase --program --file \
	$(TARGET).elf $(DEBUG_HOST):$(DEBUG_PORT)
	@$(WINSHELL) /c pause

else
	@$(WINSHELL) /c start simulavr --gdbserver --device $(MCU) --clock-freq \
	$(DEBUG_MFREQ) --port $(DEBUG_PORT)
endif
	@$(WINSHELL) /c start avr-$(DEBUG_UI) --command=$(GDBINIT_FILE)




# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT = $(OBJCOPY) --debugging
COFFCONVERT += --change-section-address .data-0x800000
COFFCONVERT += --change-section-address .bss-0x800000
COFFCONVERT += --change-section-address .noinit-0x800000
COFFCONVERT += --change-section-address .eeprom-0x810000



coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof


extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof



# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH)
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock $< $@


%.mem: %.hex
	$(SILENT) @echo
	$(SILENT) @echo $(MSG_MEM)
	$(SREC_CAT) $< -Intel -Byte_Swap 2  -Data_Only -o tmp.mem -vmem 8
	$(GAWK) ' BEGIN{FS=" ";} { $$1= ""; print}' tmp.mem > out.mem

%.bit: %.mem
	@echo
	@echo $(MSG_BIT)
	$(DATA2MEM) -bm $(GF_SP_BMM) -bt $(GF_SP_BIT) -bd out.mem -o b out.bit

#%.bit: %.elf
#	@echo
#	@echo $(MSG_BIT) $@
#	$(DATA2MEM) -bm $(GF_SP_BMM) -bt $(GF_SP_BIT) -bd $< -o b $@

	
%.dmp:
	@echo
	@echo $(MSG_DMP)
	$(DATA2MEM) -bm $(GF_SP_BMM) -bt out.bit -d > out.dmp	
	
%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@



# Create library from object files.
.SECONDARY : $(TARGET).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@echo
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(OBJ)


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 


# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(TARGET).mem
	$(REMOVE) $(TARGET).bit
	$(REMOVE) $(TARGET).dmp
	$(REMOVE) $(SRC:%.c=$(OBJDIR)/%.o)
	$(REMOVE) $(SRC:%.c=$(OBJDIR)/%.lst)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRC:.c=.i)
	$(REMOVEDIR) .dep


# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program debug gdb-config


