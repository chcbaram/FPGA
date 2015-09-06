CROSS=avr-

CC=$(CROSS)gcc
CXX=$(CROSS)g++
AR=$(CROSS)ar
OBJCOPY=$(CROSS)objcopy
SIZE=$(CROSS)size

CFLAGS=$(EXTRACFLAGS) -w -fPIC -g -ffunction-sections -fdata-sections -Os -I$(COREPATH) -mmcu=$(PREFS___board___build___mcu) -DF_CPU=$(PREFS___board___build___f_cpu) -DREVISION=$(REVISION)
CXXFLAGS=$(CFLAGS) -fno-exceptions
ARFLAGS=crs
LDFLAGS=-Wl,--gc-sections -mmcu=$(PREFS___board___build___mcu) -L$(COREPATH)

$(TARGET).elf: $(TARGETOBJ) $(LIBS)
	$(CC) -o $@ $(TARGETOBJ) $(LDFLAGS) -lm -Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive 

all-target: $(TARGET).elf $(TARGET).hex $(TARGET).eep $(TARGET).size

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).eep: $(TARGET).elf
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@

$(TARGET).size: $(TARGET).elf
	$(SIZE) $< > $@

.o:.S
	$(CC) $(CFLAGS) -assembler-with-cpp -c $+ -o $@
