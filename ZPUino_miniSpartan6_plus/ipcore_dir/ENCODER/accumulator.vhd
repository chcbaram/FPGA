----------------------------------------------------------------------------------
-- Company:   Mike Field <hamster@snap.net.nz>
--
-- Module Name:  accumulator - Behavioral 
--
-- Description: Accumulate the 'up' and 'down' pulses from a rotary encoder.
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity accumulator is
    Port ( clk   : in  STD_LOGIC;
           up    : in  STD_LOGIC;
           down  : in  STD_LOGIC;
		   dir   : out STD_LOGIC;
           value : out  STD_LOGIC_VECTOR (31 downto 0));
end accumulator;

architecture Behavioral of accumulator is
   signal total : unsigned(value'range) := (others => '0');
   signal s_dir : std_logic := '0';
begin
   value <= std_logic_vector(total);

	dir <= s_dir;

process(clk)
   begin
      if rising_edge(clk) then
         if up = down then
            null;
         elsif up = '1' then
            total <= total + 1;
			s_dir <= '1';
         else -- down must = '1'
            total <= total - 1;
			s_dir <= '0';
         end if;
      end if;
   end process;
end Behavioral;