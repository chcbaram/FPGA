OBJDIR=../Work
WBHPATH=../../wb_handler/
WORK=i2c_mwb
GHDL=ghdl
GHDLFLAGS=-P../../c_vhdl/c-obj/ -P../Work -P$(WBHPATH)Work --work=$(WORK) --workdir=$(OBJDIR)
vpath %.o $(OBJDIR)
vpath %.vhdl ..

# Default target
all: $(OBJDIR)/tb

# Targets to analyze files
%.o : %.vhdl
	#bakalint.pl --no-warn-case -i $< -r $(OBJDIR)/replace.txt -d $(OBJDIR)/$@
	$(GHDL) -a $(GHDLFLAGS) $<

# Elaboration target
$(OBJDIR)/tb: i2c_memory_pkg.o i2c_master_pkg.o tst_bench_top.o \
	i2c_memory.o i2c_master_top.o i2c_master_byte_ctrl.o \
	i2c_master_bit_ctrl.o
	$(GHDL) -e $(GHDLFLAGS) $(@F)
	mv $(@F) $(OBJDIR)

tst_bench_top.o: tst_bench_top.vhdl $(WBHPATH)wb_handler_pkg.vhdl i2c_memory_pkg.vhdl \
	i2c_master_pkg.vhdl
i2c_master_top.o: i2c_master_top.vhdl i2c_master_pkg.vhdl
i2c_master_byte_ctrl.o: i2c_master_byte_ctrl.vhdl i2c_master_pkg.vhdl
i2c_master_bit_ctrl.o: i2c_master_bit_ctrl.vhdl i2c_master_pkg.vhdl


