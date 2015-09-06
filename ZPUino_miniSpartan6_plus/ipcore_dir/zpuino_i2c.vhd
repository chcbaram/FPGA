--
--  Timers for ZPUINO
-- 
--  Copyright 2010 Alvaro Lopes <alvieboy@alvie.com>
-- 
--  Version: 1.0
-- 
--  The FreeBSD license
--  
--  Redistribution and use in source and binary forms, with or without
--  modification, are permitted provided that the following conditions
--  are met:
--  
--  1. Redistributions of source code must retain the above copyright
--     notice, this list of conditions and the following disclaimer.
--  2. Redistributions in binary form must reproduce the above
--     copyright notice, this list of conditions and the following
--     disclaimer in the documentation and/or other materials
--     provided with the distribution.
--  
--  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY
--  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
--  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
--  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
--  ZPU PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
--  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
--  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
--  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
--  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
--  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
--  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
--  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--  
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

library work;
use work.zpu_config.all;
use work.zpupkg.all;
use work.zpuinopkg.all;
use Work.I2C_Master.all;

entity zpuino_i2c is

  port (
    wb_clk_i: in std_logic;
    wb_rst_i: in std_logic;
    wb_dat_o: out std_logic_vector(wordSize-1 downto 0);
    wb_dat_i: in std_logic_vector(wordSize-1 downto 0);
    wb_adr_i: in std_logic_vector(maxIObit downto minIObit);
    wb_we_i:  in std_logic;
    wb_cyc_i: in std_logic;
    wb_stb_i: in std_logic;
    wb_ack_o: out std_logic;
    wb_inta_o:out std_logic;
    wb_intb_o:out std_logic;
    
	 
    -- i2c lines
    scl_pad_i       : in  std_logic;                -- i2c clock line input
    scl_pad_o       : out std_logic;                -- i2c clock line output
    scl_padoen_o    : out std_logic;                -- i2c clock line output enable, active low
    sda_pad_i       : in  std_logic;                -- i2c data line input
    sda_pad_o       : out std_logic;                -- i2c data line output
    sda_padoen_o    : out std_logic                 -- i2c data line output enable, active low			
	 
  );
end entity zpuino_i2c;


architecture behave of zpuino_i2c is

   signal scl_padi        : std_logic; -- i2c clock line input
   signal scl_pado        : std_logic; -- i2c clock line output
   signal scl_padoen      : std_logic; -- i2c clock line output enable, active low
   signal sda_padi        : std_logic; -- i2c data line input
   signal sda_pado        : std_logic; -- i2c data line output
   signal sda_padoen      : std_logic; -- i2c data line output enable, active low
   signal SCL             : std_logic; -- i2c clock line
   signal SDA             : std_logic; -- i2c data line
	

begin
			

  i2c_inst : I2C_MasterTop
  port map (
    wb_clk_i      => wb_clk_i,
    wb_rst_i      => wb_rst_i,
    wb_dat_o      => wb_dat_o( 7 downto 0),
    wb_dat_i      => wb_dat_i( 7 downto 0),
    wb_adr_i      => wb_adr_i( 2 downto 0),
    wb_we_i       => wb_we_i,
    wb_cyc_i      => wb_cyc_i,
    wb_stb_i      => wb_stb_i,
    wb_ack_o      => wb_ack_o,
    wb_inta_o     => wb_inta_o,
		
    -- i2c lines
    scl_pad_i    => scl_pad_i, 
	 scl_pad_o    => scl_pad_o, 
	 scl_padoen_o => scl_padoen_o,
    sda_pad_i    => sda_pad_i, 
	 sda_pad_o    => sda_pad_o, 
	 sda_padoen_o => sda_padoen_o
  );
  
  
end behave;
