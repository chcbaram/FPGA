Copyright (c) 2005-2006 Salvador E. Tropea <salvador en inti gov ar>
Copyright (c) 2005-2006 Instituto Nacional de Tecnología Industrial
Copyright (c) 2000 Richard Herveille <richard@asics.ws>
Original license: Free distribution, keep (c) in the sources.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA

Dependencies: bakalint (optional)
              xtracth (.h & .inc generator)
  For the testbench:
              c.stdio_h (C library for VHDL)
              wb_handler.WishboneTB (Wishbone Handler)
              ghdl

Library name: i2c_mwb (I²C Master Wishbone)

  That's a core for an I²C Wishbone Master peripheral.
  The original core was from OpenCores and it have many modifications
oriented to reduce the area and increase the speed.
  Most changes can be "reverted" using generics.
  The testbench includes an I²C memory (Slave) that behaves quite similar to
24Lxx parts. The original code is from the same author but in Verilog.
  The makefile generates a .h and a .inc with register's definitions.
  In the doc directory you'll find a PDF with information about the core and
the protocol. Some stuff is out-dated because we moved some registers. The
main idea was to need only two address lines when selecting a fixed prescaler.
  The "DEBUG" registers are optional because they are needed only to debug this
core (not the core using it).

If you use ghdl:
  To create the library and the testbench just use make.
  To execute the testbench run: Work/tb

Registers:
----------

All are 8 bits. That's a resume and its objetive is to show the differents
between the actual core and the documentation.

Address   | Mode | Name        | Description
-----------------------------------------------------------------------------
    0     |  R/W | I2C_CTR     | Control Register
          |      |             | bit 7: Enable (1=enabled)
          |      |             | bit 6: Interrupt Enable (1=enable)
          |      |             |        Not used when USE_IEN is false.
-----------------------------------------------------------------------------
    1     |   R  | I2C_RXR     | Receive Register
-----------------------------------------------------------------------------
    1     |   W  | I2C_TXR     | Transmit Register
-----------------------------------------------------------------------------
    2     |   R  | I2C_SR      | Status Register
          |      |             | bit 7: Receive Acknowledge (0=Ack received)
          |      |             | bit 6: I²C bus busy (1=START detected)
          |      |             | bit 5: Arbitration Lost
          |      |             | bit 1: Transfer in Progress (1=transmiting)
          |      |             | bit 0: Interrupt Flag (end of transmision
          |      |             |        or arbitration lost)
-----------------------------------------------------------------------------
    2     |   W  | I2C_CR      | Command Register
          |      |             | bit 7: Generate START
          |      |             | bit 6: Generate STOP
          |      |             | bit 5: Read from Slave
          |      |             | bit 4: Write to Slave
          |      |             | bit 3: Acknowledge
          |      |             | bit 0: Interrupt Ack
-----------------------------------------------------------------------------
    3     |   W  | I2C_PRER_LO | Clock Prescaler Low (only if not using
          |      |             | FIXED_PRER)
-----------------------------------------------------------------------------
    4     |   W  | I2C_PRER_HI | Clock Prescaler High (only if not using
          |      |             | FIXED_PRER)
-----------------------------------------------------------------------------
    5     |   R  | I2C_UTXR_R  | TXR Readback (only for DEBUG==true)
-----------------------------------------------------------------------------
    6     |   R  | I2C_UCR_R   | CR Readback (only for DEBUG==true)
-----------------------------------------------------------------------------
    
Generics:
---------

ARST_LVL   : std_logic:='0'; -- Asynchronous reset level
DEBUG      : boolean:=false; -- Enable debug registers I2C_UTXR_R & I2C_UCR_R
MUX_BETTER : boolean:=false; -- True if using MUX is better than using
                             -- tri-states
FULL_SYNC  : boolean:=false; -- True if you need full synchronous behavior,
                             -- introduces 1 WS to Wishbone.
FIXED_PRER : integer:=-1;    -- Assigning a value removes the PRER and uses
                             -- it as pre-scaler
USE_IEN    : boolean:=true   -- False if interrupts are always enabled
                             -- (masked in another component)

