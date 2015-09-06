------------------------------------------------------------------------------
----                                                                      ----
----  I2C Master Testbench                                                ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----                                                                      ----
----  Description:                                                        ----
----  I2C module test bench. Connects a Wishbone handler and an I2C       ----
---- memory to the module. Then it makes some write/read operations.      ----
----  The test cases are the same performed by the Verilog test bench by  ----
---- Richard Herveille.                                                   ----
----                                                                      ----
----  To Do:                                                              ----
----  -                                                                   ----
----                                                                      ----
----  Author:                                                             ----
----    - Salvador E. Tropea, salvador en inti gov ar                     ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Copyright (c) 2005 Salvador E. Tropea <salvador en inti gov ar>      ----
---- Copyright (c) 2005 Instituto Nacional de Tecnología Industrial       ----
----                                                                      ----
---- Covered by the GPL license.                                          ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Design unit:      TB(TestBench) (Entity and architecture)            ----
---- File name:        tst_bench_top.vhdl                                 ----
---- Note:             None                                               ----
---- Limitations:      None known                                         ----
---- Errors:           None known                                         ----
---- Library:          i2c_mwb                                            ----
---- Dependencies:     IEEE.std_logic_1164                                ----
----                   IEEE.numeric_std                                   ----
----                   c.stdio_h                                          ----
----                   wb_handler.WishboneTB                              ----
----                   irq_ctrl.I2C_TB                                    ----
----                   irq_ctrl.I2C_Master                                ----
---- Target FPGA:      None                                               ----
---- Language:         VHDL                                               ----
---- Wishbone:         None                                               ----
---- Synthesis tools:  None                                               ----
---- Simulation tools: GHDL [Sokcho edition] (0.1x)                       ----
---- Text editor:      SETEdit 0.5.x                                      ----
----                                                                      ----
------------------------------------------------------------------------------
--
-- CVS Revision History
--
-- $Log: tst_bench_top.vhdl,v $
-- Revision 1.11  2006/04/17 19:44:43  salvador
-- * Modified: License to GPL.
--
-- Revision 1.10  2005/05/20 14:39:05  salvador
-- * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
-- Revision 1.9  2005/05/18 14:50:20  salvador
-- * Modificado: Los encabezados de los archivos para que cumplan con nuestras
-- recomendaciones.
--
--
--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
library c;
use c.stdio_h.all;
library wb_handler;
use wb_handler.WishboneTB.all;
library Work;
use Work.I2C_TB.all;
use Work.I2C_Master.all;

entity TB is
end entity TB;

architecture TestBench of TB is
   -- Clock=100000 KHz
   constant CLKPERIOD     : time:=10 us;
   signal delay           : integer:=5;
   signal veces           : integer:=0;
   signal addr            : std_logic_vector(8-1 downto 0);
   signal datai           : std_logic_vector(8-1 downto 0);
   signal datao           : std_logic_vector(8-1 downto 0);
   signal rd              : std_logic:='0';
   signal wr              : std_logic:='0';
   signal rde             : std_logic;
   signal wre             : std_logic;
   signal wb_rst          : std_logic:='1';
   signal wb_clk          : std_logic;--:='0';
   signal wb_adr          : std_logic_vector(8-1 downto 0);
   signal adr_o3          : unsigned(2 downto 0);
   signal wb_dati         : std_logic_vector(8-1 downto 0):=(others => 'Z');
   signal wb_dato         : std_logic_vector(8-1 downto 0);
   signal wb_we           : std_logic;
   signal wb_stb          : std_logic;
   signal wb_cyc          : std_logic;
   signal wb_ack          : std_logic:='0';
   signal inta            : std_logic;

   signal scl_padi        : std_logic; -- i2c clock line input
   signal scl_pado        : std_logic; -- i2c clock line output
   signal scl_padoen      : std_logic; -- i2c clock line output enable, active low
   signal sda_padi        : std_logic; -- i2c data line input
   signal sda_pado        : std_logic; -- i2c data line output
   signal sda_padoen      : std_logic; -- i2c data line output enable, active low
   signal scl             : std_logic; -- i2c clock line
   signal sda             : std_logic; -- i2c data line

   signal sdar            : std_logic; -- i2c data line
   signal sclr            : std_logic; -- i2c data line

   constant PREV_LO       : std_logic_vector(8-1 downto 0):="00000001";
   constant PREV_HI       : std_logic_vector(8-1 downto 0):="00000000";

   constant RD_B          : std_logic:='1';
   constant WR_B          : std_logic:='0';
   constant SADR          : std_logic_vector(6 downto 0):="0010000";

   --Constant  FIXED_PRER : integer:=-1;
   constant FIXED_PRER    : integer:=1;

   procedure DelayCk(signal wb_clk_i: in std_logic;
                     constant TIMES : in integer) is
      variable count : integer;
   begin
      count:=TIMES;
      while count>0 loop
         wait until rising_edge(wb_clk_i);
         count:=count-1;
      end loop;
   end procedure DelayCk;

   procedure CheckTIP(signal addr_o : out std_logic_vector(8-1 downto 0);
                      signal data_i : in  std_logic_vector(8-1 downto 0);
                      signal rd_o   : out std_logic;
                      signal rde_i  : in  std_logic;
                      constant ACKV : in boolean:=true) is
   begin
      WBRead(addr_o,I2C_SR,rd_o,rde_i);
      while data_i(1)='1' loop
         WBRead(addr_o,I2C_SR,rd_o,rde_i);
      end loop;
      if ACKV then
         assert data_i(7)='0' report "Expected ACK, received NACK"
            severity failure;
      else
         assert data_i(7)='1' report "Expected NACK, received ACK"
            severity failure;
      end if;
   end procedure CheckTIP;
begin
   -- Clock
   clock_generator:
   process
   begin
      wb_clk <= '0';
      wait for CLKPERIOD/2;
      wb_clk <= '1';
      wait for CLKPERIOD/2;
      veces <= veces+1;
      if veces=1500 then
         assert false report "Fin de la simulación" severity failure;
      end if;
   end process clock_generator;

   -- Reset pulse
   p_reset:
   process
   begin
      wb_rst <= '1';
      wait until rising_edge(wb_clk);
      wb_rst <= '0' after 500 ns;
      wait;
   end process p_reset;

   -- Wishbone master simulator
   wb_master: WBHandler
      port map(
         addr_i => addr, data_i => datai, data_o => datao,
         rd_i => rd, wr_i => wr, rde_o => rde, wre_o => wre,
         wb_rst_i => wb_rst, wb_clk_i => wb_clk,  wb_ack_i => wb_ack,
         wb_adr_o => wb_adr, wb_dat_i => wb_dati, wb_dat_o => wb_dato,
         wb_we_o  => wb_we,  wb_stb_o => wb_stb,  wb_cyc_o => wb_cyc);

   -- I2C module (Wishbone slave, I2C master)
   adr_o3 <= unsigned(wb_adr(2 downto 0));
   wb_i2c: I2C_MasterTop
      generic map(DEBUG => false,
         FULL_SYNC => false,
         FIXED_PRER => FIXED_PRER)
      port map(-- Wishbone signals
         wb_clk_i => wb_clk,  wb_rst_i => wb_rst,  wb_adr_i => adr_o3,
         wb_dat_i => wb_dato, wb_dat_o => wb_dati, wb_we_i  => wb_we,
         wb_stb_i => wb_stb,  wb_cyc_i => wb_cyc,  wb_ack_o => wb_ack,
         wb_inta_o => inta,
         -- i2c lines
         scl_pad_i => scl_padi, scl_pad_o => scl_pado, scl_padoen_o => scl_padoen,
         sda_pad_i => sda_padi, sda_pad_o => sda_pado, sda_padoen_o => sda_padoen);

   -- SCL/SDA pads
   scl <= 'Z' when scl_padoen='1' else scl_pado;-- after 600 ns;
   scl_padi <= sclr;
   scl <= 'H'; -- Pull-up
   sda <= 'Z' when sda_padoen='1' else sda_pado;-- after 600 ns;
   sda_padi <= sdar;
   sda <= 'H'; -- Pull-up

   -- SCL/SDA values converted to X/0/1/Z
   -- The I2C module can't handle 'H'
   sdar <= to_x01z(sda);
   sclr <= to_x01z(scl);

   -- I2C memory (I2C slave)
   i2c_mem: I2C_Memory
      generic map(
         I2C_ADR => SADR, DEBUG => false)
      port map(
         scl_x_i => scl, sda_x_io => sda, rst_i => wb_rst);

   sequence:
   process
   begin
      wait until wb_rst='0';

      if FIXED_PRER=-1 then
         -- load prescaler lo-byte
         DelayCk(wb_clk,1);
         WBWrite(addr,I2C_PRER_LO,datai,PREV_LO,wr,wre);
         -- load prescaler hi-byte
         WBWrite(addr,I2C_PRER_HI,datai,PREV_HI,wr,wre);
         assert false report "Programmed registers" severity note;

         -- verify
         WBRead(addr,I2C_PRER_LO,rd,rde);
         if not(datao=PREV_LO) then
            printf("Prescaler lo: %s\n",datao);
            assert false report "Wrong prescaler lo readback" severity failure;
         end if;
         WBRead(addr,I2C_PRER_HI,rd,rde);
         assert datao=PREV_HI report "Wrong prescaler hi readback" severity failure;
      else
         assert false report "Using fixed pre-scaler" severity note;
      end if;

      -- enable core
      WBWrite(addr,I2C_CTR,datai,"10000000",wr,wre); -- 0x80
      assert false report "Core Enabled" severity note;

      -- present slave address, set write-bit
      WBWrite(addr,I2C_TXR,datai,SADR & WR_B,wr,wre); -- 0x20
      -- set command (start, write)
      WBWrite(addr,I2C_CR,datai,"10010000",wr,wre); -- 0x90
      assert false report "Generate Start" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- send memory address
      -- present slave's memory address
      WBWrite(addr,I2C_TXR,datai,"00000001",wr,wre); -- 0x01
      -- set command (write)
      WBWrite(addr,I2C_CR,datai,"00010000",wr,wre); -- 0x10
      assert false report "Write slave memory address 01" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- send memory contents
      -- present data
      WBWrite(addr,I2C_TXR,datai,"10100101",wr,wre); -- 0xA5
      -- set command (write)
      WBWrite(addr,I2C_CR,datai,"00010000",wr,wre); -- 0x10
      assert false report "Write data 0xA5" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- send memory contents for next memory address (auto_inc)
      -- present data
      WBWrite(addr,I2C_TXR,datai,"01011010",wr,wre); -- 0x5A
      -- set command (stop, write)
      WBWrite(addr,I2C_CR,datai,"01010000",wr,wre); -- 0x50
      assert false report "Write next data 0x5A, generate 'stop'" severity note;

      CheckTIP(addr,datao,rd,rde);

      DelayCk(wb_clk,30);

      -- present slave address, set write-bit
      WBWrite(addr,I2C_TXR,datai,SADR & WR_B,wr,wre); -- 0x20
      -- set command (start, write)
      WBWrite(addr,I2C_CR,datai,"10010000",wr,wre); -- 0x90
      assert false report "Generate Start" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- send memory address
      -- present slave's memory address
      WBWrite(addr,I2C_TXR,datai,"00000001",wr,wre); -- 0x01
      -- set command (write)
      WBWrite(addr,I2C_CR,datai,"00010000",wr,wre); -- 0x10
      assert false report "Write slave memory address 01" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- present slave address, set read-bit
      WBWrite(addr,I2C_TXR,datai,SADR & RD_B,wr,wre); -- 0x21
      -- set command (start, write)
      WBWrite(addr,I2C_CR,datai,"10010000",wr,wre); -- 0x90
      assert false report "Generate 'Repeated Start' (RD)" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- read data from slave
      -- set command (read, ack_read)
      WBWrite(addr,I2C_CR,datai,"00100000",wr,wre); -- 0x20
      assert false report "Sending read + ack" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- check data just received
      WBRead(addr,I2C_RXR,rd,rde);
      if unsigned(datao)=16#A5# then
         assert false report "Received ok" severity note;
      else
         printf("Wrong value: %s\n",datao);
         assert false report "Wrong value received" severity failure;
      end if;

      -- read data from slave
      -- set command (read, ack_read)
      WBWrite(addr,I2C_CR,datai,"00100000",wr,wre); -- 0x20
      assert false report "Sending read + ack" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- check data just received
      WBRead(addr,I2C_RXR,rd,rde);
      if unsigned(datao)=16#5A# then
         assert false report "Received ok" severity note;
      else
         printf("Wrong value: %s\n",datao);
         assert false report "Wrong value received" severity failure;
      end if;

      -- read data from slave
      -- set command (read, ack_read)
      WBWrite(addr,I2C_CR,datai,"00100000",wr,wre); -- 0x20
      assert false report "Sending read + ack" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- check data just received
      WBRead(addr,I2C_RXR,rd,rde);
      printf("Received 0x%X from 3rd read address\n",to_integer(unsigned(datao)));

      -- read data from slave
      -- set command (read, ack_read)
      WBWrite(addr,I2C_CR,datai,"00100000",wr,wre); -- 0x20
      assert false report "Sending read + ack" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- check data just received
      WBRead(addr,I2C_RXR,rd,rde);
      printf("Received 0x%X from 4th read address\n",to_integer(unsigned(datao)));

      -- present slave address, set read-bit
      WBWrite(addr,I2C_TXR,datai,SADR & WR_B,wr,wre); -- 0x20
      -- set command (start, write)
      WBWrite(addr,I2C_CR,datai,"10010000",wr,wre); -- 0x90
      assert false report "Generate 'Repeated Start' (RD)" severity note;

      CheckTIP(addr,datao,rd,rde);

      -- send memory address
      -- present slave's memory address
      WBWrite(addr,I2C_TXR,datai,"00010000",wr,wre); -- 0x10
      -- set command (write)
      WBWrite(addr,I2C_CR,datai,"00010000",wr,wre); -- 0x10
      assert false report "Write slave memory address 0x10" severity note;

      CheckTIP(addr,datao,rd,rde,false);
      assert false report "OK, NACK received" severity note;

      -- set command (stop)
      WBWrite(addr,I2C_CR,datai,"01000000",wr,wre); -- 0x40
      assert false report "Sending stop" severity note;

      wait;
   end process sequence;

end architecture TestBench; -- of entity TB
