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

entity zpuino_encoder is
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

	ENCODER_A    : in    STD_LOGIC_VECTOR( 1 downto 0 );
	ENCODER_B    : in    STD_LOGIC_VECTOR( 1 downto 0 )
  );
end entity zpuino_encoder;


architecture behave of zpuino_encoder is


	COMPONENT accumulator
	PORT(
      clk : IN std_logic;
      up : IN std_logic;
      down : IN std_logic;          
	  dir   : out std_logic;
      value : OUT std_logic_vector(31 downto 0)
      );
	END COMPONENT;

	COMPONENT encoder
	PORT(
      clk : IN std_logic;
      quad : IN std_logic_vector(1 downto 0);          
      up : OUT std_logic;
      down : OUT std_logic;
      error : OUT std_logic
      );
	END COMPONENT;
   

	signal s_encoder_count_A : std_logic_vector(31 downto 0);
	signal s_encoder_count_B : std_logic_vector(31 downto 0);


	signal up_A		: std_logic;
	signal down_A 	: std_logic;
	signal up_B		: std_logic;
	signal down_B 	: std_logic;
	
	signal dir_A 	: std_logic;
	signal dir_B 	: std_logic;

	
	
begin

	wb_ack_o <= wb_cyc_i and wb_stb_i;
	wb_inta_o <= '0';


	 

	-- Read
	--
	process(wb_adr_i)
	begin
	  case wb_adr_i(5 downto 2) is
		when X"0" =>
			wb_dat_o <= (others=>'0');
			wb_dat_o(0) <= dir_A;
		when X"1" =>
			wb_dat_o <= s_encoder_count_A;

		when X"2" =>
			wb_dat_o <= (others=>'0');
			wb_dat_o(0) <= dir_B;
		when X"3" =>
			wb_dat_o <= s_encoder_count_B;
			
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
						--frame_addr <= wb_dat_i( 18 downto 0 );
					when X"1" =>
					when others =>
				end case;
			end if;
		end if;
	end process;


   enc_acc_A: accumulator PORT MAP(
      clk   => wb_clk_i,
      up    => up_A,
      down  => down_A,
	  dir   => dir_A,
      value => s_encoder_count_A
   );

   enc_A: encoder PORT MAP(
      clk     => wb_clk_i,
      quad(0) => ENCODER_A(0),
	  quad(1) => ENCODER_B(0),
      up      => up_A,
      down    => down_A,
      error   => open
   );
	

   enc_acc_B: accumulator PORT MAP(
      clk   => wb_clk_i,
      up    => up_B,
      down  => down_B,
	  dir   => dir_B,
      value => s_encoder_count_B
   );

   enc_B: encoder PORT MAP(
      clk     => wb_clk_i,
      quad(0) => ENCODER_A(1),
	  quad(1) => ENCODER_B(1),
      up      => up_B,
      down    => down_B,
      error   => open
   );
  
end behave;

