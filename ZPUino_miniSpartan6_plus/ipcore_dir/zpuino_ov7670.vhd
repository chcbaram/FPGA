--
--  ADC for ZPUINO
-- 
--  
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
use ieee.numeric_std.all;
use IEEE.std_logic_unsigned.all; 

library work;
use work.zpu_config.all;
use work.zpupkg.all;
use work.zpuinopkg.all;
use work.zpuino_config.all;

entity zpuino_ov7670 is
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

	CLK50        : in    STD_LOGIC;
	OV7670_VSYNC : in    STD_LOGIC;
	OV7670_HREF  : in    STD_LOGIC;
	OV7670_PCLK  : in    STD_LOGIC;
	OV7670_XCLK  : out   STD_LOGIC;
	OV7670_D     : in    STD_LOGIC_VECTOR(7 downto 0)

  );
end entity zpuino_ov7670;


architecture behave of zpuino_ov7670 is

	COMPONENT ov7670_capture
	PORT(
		pclk  : IN std_logic;
		vsync : IN std_logic;
		href  : IN std_logic;
		d     : IN std_logic_vector(7 downto 0);          
		addr  : OUT std_logic_vector(18 downto 0);
		dout  : OUT std_logic_vector(15 downto 0);
		we    : OUT std_logic
		);
	END COMPONENT;

	COMPONENT frame_buffer
	  PORT (
		clka : IN STD_LOGIC;
		wea : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
		addra : IN STD_LOGIC_VECTOR(14 DOWNTO 0);
		dina : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		clkb : IN STD_LOGIC;
		addrb : IN STD_LOGIC_VECTOR(14 DOWNTO 0);
		doutb : OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
	  );
	END COMPONENT;

	signal capture_addr    : std_logic_vector(18 downto 0);
	signal capture_data    : std_logic_vector(15 downto 0);
	signal capture_we      : std_logic_vector(0 downto 0);
   
	signal frame_addr      : std_logic_vector(18 downto 0);
	signal frame_pixel     : std_logic_vector(15 downto 0);
	
	
	signal ov7670_sys_clk	: std_logic := '0';  

	
begin

	wb_ack_o <= wb_cyc_i and wb_stb_i;
	wb_inta_o <= '0';


	process(CLK50)
	begin
		if rising_edge(CLK50) then
			ov7670_sys_clk <= not ov7670_sys_clk;
		end if;
	end process;
	
	OV7670_XCLK <= ov7670_sys_clk;
	
	
	 

	-- Read
	--
	process(wb_adr_i)
	begin
	  case wb_adr_i(5 downto 2) is
		when X"0" =>
				wb_dat_o <= (others=>'0');
				wb_dat_o( 15 downto 0 ) <= frame_pixel;
		when X"1" =>
		when others =>
		  wb_dat_o <= (others => DontCareValue);
	  end case;
	end process;


	-- Write
	--
	process(wb_clk_i)
	begin
		if rising_edge(wb_clk_i) then
			if wb_rst_i='1' then
			
			elsif wb_stb_i='1' and wb_cyc_i='1' and wb_we_i='1' then
				case wb_adr_i(5 downto 2) is
					when X"0" =>
						frame_addr <= wb_dat_i( 18 downto 0 );
					when X"1" =>
					when others =>
				end case;
			end if;
		end if;
	end process;



capture: ov7670_capture 
	PORT MAP(
		pclk  => OV7670_PCLK,
		vsync => OV7670_VSYNC,
		href  => OV7670_HREF,
		d     => OV7670_D,
		addr  => capture_addr,
		dout  => capture_data,
		we    => capture_we(0)
	);
	

fb : frame_buffer
	PORT MAP (
		clka  => OV7670_PCLK,		
		wea   => capture_we,
		addra => capture_addr( 14 downto 0 ),
		dina  => capture_data,
	 
		clkb  => wb_clk_i,
		addrb => frame_addr( 14 downto 0 ),
		doutb => frame_pixel
  );

  
end behave;

