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

entity zpuino_adc is
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

    AD_CS_o  : out std_logic; 
	 AD_SCLK_o: out std_logic;
    AD_DIN_o : out std_logic; 
    AD_OUT_i : in  std_logic
  );
end entity zpuino_adc;


architecture behave of zpuino_adc is

signal adc_data_0:    std_logic_vector(15 downto 0); 
signal adc_data_1:    std_logic_vector(15 downto 0); 
signal adc_data_2:    std_logic_vector(15 downto 0); 
signal adc_data_3:    std_logic_vector(15 downto 0); 
signal adc_data_4:    std_logic_vector(15 downto 0); 
signal adc_data_5:    std_logic_vector(15 downto 0);
signal adc_data_6:    std_logic_vector(15 downto 0);



begin

wb_ack_o <= wb_cyc_i and wb_stb_i;
wb_inta_o <= '0';



AD_CS_o		<= '1';
AD_SCLK_o	<= '1';
AD_DIN_o		<= '1';
	 

-- Read
--
process(wb_adr_i)
begin
  case wb_adr_i(5 downto 2) is
    when X"0" =>
			wb_dat_o <= (others=>'0');
			wb_dat_o( 7 downto 0 ) <= X"01";
    when X"1" =>
			wb_dat_o <= (others=>'0');
			wb_dat_o( 7 downto 0 ) <= X"03";
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
				when X"2" =>
            when X"3" =>
					--<= wb_dat_i;
            when others =>
			end case;
		end if;
	end if;
end process;

end behave;

