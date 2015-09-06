CROSS=zpu-elf-

CC=$(CROSS)gcc
CXX=$(CROSS)g++
AR=$(CROSS)ar
OBJCOPY=$(CROSS)objcopy
SIZE=$(CROSS)size
DEPDIR=.deps

EXTRACFLAGS+=$(PREFS___board___build___extraCflags)

CFLAGS=$(EXTRACFLAGS) -DZPU -DZPUINO_ACCEL -Wall -O2 -fno-reorder-blocks  -fno-reorder-blocks-and-partition \
          -fno-prefetch-loop-arrays  -fno-gcse -ffunction-sections -fdata-sections -nostartfiles -mmult -mdiv -mno-callpcrel -mno-pushspadd -mno-poppcrel -I$(COREPATH)
CXXFLAGS=$(CFLAGS) -fno-exceptions -fno-rtti
ARFLAGS=crs
LDFLAGS=-O2 -nostartfiles -Wl,-T -Wl,$(COREPATH)/zpuino.lds -Wl,--relax -Wl,--gc-sections
LDFLAGSR=-O2 -nostartfiles -Wl,-T -Wl,$(COREPATH)/zpuino-r.lds

ASFLAGS=$(CFLAGS) $(PREFS___board___build___extraSflags) -DASSEMBLY

$(TARGET).elf: $(TARGETOBJ) $(LIBS)
	$(CC) -o $@ $(TARGETOBJ) $(LDFLAGS) -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive

$(TARGET)-r.elf: $(TARGETOBJ) $(LIBS)
	$(CC) -o $@ $(TARGETOBJ) $(LDFLAGSR) -r -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive

all-target: $(TARGET).elf $(TARGET).bin $(TARGET).size $(TARGET)-r.elf 

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).size: $(TARGET).elf
	$(SIZE) $< > $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXCOMPILE) -MT $@ -MD -MP -MF ./$(DEPDIR)/$*.Tpo -c -o $@ "$<"
