----------------------------------------------------------------------------------
-- Engineer:  Mike Field <hamster@snap.net.nz>
-- 
-- Module Name:    pmodenc - Behavioral 
-- Description:    Process the quadrature signals from the rotary encode on 
--                 the Digilent PMODENC 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity encoder is
    Port ( clk   : in  STD_LOGIC;
           quad  : in  STD_LOGIC_VECTOR (1 downto 0);
           up    : out  STD_LOGIC;
           down  : out  STD_LOGIC;
           error : out  STD_LOGIC);
end encoder;

architecture Behavioral of encoder is
   signal sr : std_logic_vector(5 downto 0) := (others => '0');
begin

process(clk)
   begin
      if rising_edge(clk) then
         up    <= '0';
         down  <= '0';
         error <= '0';
         case sr(3 downto 0) is
            when "0100" => down <= '1';
            when "1101" => down <= '1';
            when "1011" => down <= '1';
            when "0010" => down <= '1';

            when "1000" => up <= '1';
            when "1110" => up <= '1';
            when "0111" => up <= '1';
            when "0001" => up <= '1';
            
            when "0011" => error <= '1';
            when "1100" => error <= '1';
            when "0110" => error <= '1';
            when "1001" => error <= '1';
            
            when others =>
         end case;
         sr <= quad & sr(sr'high downto 2);
      end if;
   end process;
end Behavioral;