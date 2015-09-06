OBJDIR=Work
VERSION=1.0.1
WORK=i2c_mwb
PKG=$(WORK)-$(VERSION)
VHDL_C=ghdl -a -P../c_vhdl/c-obj/ --work=$(WORK) --workdir=$(OBJDIR)
PREFIX=/usr/local/lib/gcc/i686-pc-linux-gnu/3.4.2/vhdl/lib/v93

.PHONY: $(OBJDIR)/tb

all: $(OBJDIR) $(OBJDIR)/i2c_master_pkg.o \
	$(OBJDIR)/i2c_master_bit_ctrl.o $(OBJDIR)/i2c_master_byte_ctrl.o \
	$(OBJDIR)/i2c_master_top.o $(OBJDIR)/tb \
	i2c_master.h

$(OBJDIR)/%.o: %.vhdl
	#bakalint.pl --no-warn-case -i $< -r $(OBJDIR)/replace.txt -d $(OBJDIR)/$@
	$(VHDL_C) $<

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/tb:
	$(MAKE) -C testbench

i2c_master.h: i2c_master_pkg.vhdl
	xtracth.pl i2c_master_pkg.vhdl

i2c_master_top.o: i2c_master_pkg.vhdl i2c_master_top.vhdl

clean:
	-rm -r $(OBJDIR)

install:
	install -d -m 0755 $(PREFIX)/$(WORK)
	install -m 0644 $(OBJDIR)/* $(PREFIX)/$(WORK)

tarball:
# 	-mkdir $(WORK) 2> /dev/null
# 	cd ../.. ; cp $(shell cat lista) fpgacores/$(WORK)/$(WORK)
# 	cp /usr/share/common-licenses/GPL $(WORK)/LICENSE
# 	-rm -f $(WORK)/Changelog $(WORK)/Notas.txt $(WORK)/lista $(WORK)/.cvsignore $(WORK)/.*.dst $(WORK)/*.epr 2> /dev/null
# 	tar zcvf ../$(PKG).tar.gz $(WORK)
# 	rm -r $(WORK)
	cd .. ; perl gentarball.pl I2C $(WORK) $(VERSION)

