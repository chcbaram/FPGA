------------------------------------------------------------------------------
----                                                                      ----
----  I2C Memory Simulator (slave)                                        ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----  Based on code from: http://www.opencores.org/projects/i2c/          ----
----                                                                      ----
----  Description:                                                        ----
----  I2C memory simulator. The sda_x and scl_x signals should be         ----
---- connected to "pull-ups" using 'H'.                                   ----
----  This code is a translation of Verilog code from OpenCores.          ----
----                                                                      ----
----                                                                      ----
----  To Do:                                                              ----
----  -                                                                   ----
----                                                                      ----
----  Authors:                                                            ----
----    - Richard Herveille, richard@asics.ws (Verilog) www.asics.ws      ----
----    - John Sheahan, jrsheahan@optushome.com.au                        ----
----    - Salvador E. Tropea, salvador en inti gov ar (VHDL translation)  ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Copyright (c) 2005 Salvador E. Tropea <salvador en inti gov ar>      ----
---- Copyright (c) 2005 Instituto Nacional de Tecnología Industrial       ----
---- Copyright (c) 2001,2002 Richard Herveille  <richard@asics.ws>        ----
----                                                                      ----
---- Covered by the GPL license.                                          ----
----                                                                      ----
---- Original distribution policy:                                        ----
----     This source file may be used and distributed without             ----
----     restriction provided that this copyright statement is not        ----
----     removed from the file and that any derivative work contains      ----
----     the original copyright notice and the associated disclaimer.     ----
----                                                                      ----
----         THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY          ----
----     EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED        ----
----     TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS        ----
----     FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR           ----
----     OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,              ----
----     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES         ----
----     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE        ----
----     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR             ----
----     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF       ----
----     LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT       ----
----     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT       ----
----     OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE              ----
----     POSSIBILITY OF SUCH DAMAGE.                                      ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Design unit:      I2C_Memory(Simulator) (Entity and architecture)    ----
---- File name:        i2c_memory.vhdl                                    ----
---- Note:             None                                               ----
---- Limitations:      None known                                         ----
---- Errors:           None known                                         ----
---- Library:          i2c_mwb                                            ----
---- Dependencies:     IEEE.std_logic_1164                                ----
----                   IEEE.numeric_std                                   ----
----                   c.stdio_h                                          ----
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
-- $Log: i2c_memory.vhdl,v $
-- Revision 1.6  2006/04/17 19:44:43  salvador
-- * Modified: License to GPL.
--
-- Revision 1.5  2005/05/20 14:39:05  salvador
-- * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
-- Revision 1.4  2005/05/18 14:50:20  salvador
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

entity I2C_Memory is
   generic(
      -- I2C device address
      I2C_ADR         : std_logic_vector(6 downto 0):="0010000";
      -- To make it more verbose
      DEBUG           : boolean:=false);
   port(
      -- I2C Clock signal, needs a pull-up ('H')
      scl_x_i         : in    std_logic;
      -- I2C Data signal, needs a pull-up ('H')
      sda_x_io        : inout std_logic;
      -- Asynchronous reset
      rst_i           : in    std_logic);
end entity I2C_Memory;

architecture Simulator of I2C_Memory is
   type memory is array (3 downto 0) of std_logic_vector(7 downto 0);
   signal mem             : memory;
   signal mem_adr         : unsigned(1 downto 0); -- memory address
   signal mem_do          : std_logic_vector(7 downto 0); -- memory data output
   signal sta, d_sta      : std_logic;
   signal sto, d_sto      : std_logic;
   signal sr              : std_logic_vector(7 downto 0):=(others => '0'); -- 8bit shift register
   signal rw              : std_logic;  -- read/write direction
   signal i2c_reset       : std_logic;  -- i2c-statemachine reset
   signal bit_cnt         : unsigned(2 downto 0):=(others => '0'); -- 3bit downcounter
   signal acc_done        : boolean;    -- 8bits transfered
   signal ld              : std_logic;  -- load downcounter
   signal sda_o           : std_logic:='1';  -- sda-drive level
   signal scl             : std_logic;
   signal sda             : std_logic;
   -- statemachine declaration
   type   states is (idle, slave_ack, get_mem_adr, gma_ack, data, data_ack);
   signal state           : states:=idle;
begin
   Shift_Register:
   process
   begin
      wait until scl'event;
      if scl='1' and rst_i='0' then
         --Wait for 1 us;
         sr <= sr(6 downto 0) & sda;
      end if;
   end process Shift_Register;

   Bit_Counter:
   process
   begin
      wait until scl'event or rst_i'event;
      if rst_i='1' then
         bit_cnt <= "000";
      elsif scl'event and scl='1' then
         if ld='1' then
            --Wait for 1 us;
            bit_cnt <= "111";
         else
            --Wait for 1 us;
            bit_cnt <= bit_cnt-1;
         end if;
      end if;
   end process Bit_Counter;

   -- Access done signal
   acc_done <= bit_cnt="000";

   Detect_Start_Condition:
   process
   begin
      wait until sda'event;
      if sda='0' then
         if scl='1' then
            if DEBUG then
               assert false report "Start condition" severity note;
            end if;
            sta   <= '1';
            d_sta <= '0';
         else
            --Wait for 1 us;
            sta <= '0';
         end if;
      else -- sda=1 rising edge
         -- Stop condition
         if scl='1' then
            sta <= '0';
         end if;
      end if;
   end process Detect_Start_Condition;

   Delayed_Start:
   process
   begin
      wait until scl'event;
      if scl='1' then
         --Wait for 1 us;
         d_sta <= sta;
      end if;
   end process Delayed_Start;

   Detect_Stop_Condition:
   process
   begin
      wait until sda'event;
      if sda='1' then
         if scl='1' then
            if DEBUG then
               assert false report "Stop condition" severity note;
            end if;
            --Wait for 1 us;
            sto <= '1';
         else
            --Wait for 1 us;
            sto <= '0';
         end if;
      else -- sda=0 falling edge
         -- Start condition
         if scl='1' then
            --Wait for 1 us;
            sto <= '0';
         end if;
      end if;
   end process Detect_Stop_Condition;

   i2c_reset <= sta or sto;

   Statemachine:
   process
      variable my_adr : boolean:=false;
   begin
      wait until scl'event or sto'event;
      if scl='0' or sto='1' then
         if sto='1' or (sta='1' and d_sta='0') then
            if DEBUG then
               assert false report "Reset statemachine" severity note;
            end if;
            --Wait for 1 us;
            state <= idle; -- reset statemachine
            sda_o <= '1';
            ld    <= '1';
            my_adr:=false;
         else
            --Wait for 1 us;
            -- Initial settings
            sda_o <= '1';
            ld    <= '0';

            case state is
                 when idle => -- idle state
                      if acc_done and not(my_adr) then
                         my_adr:=sr(7 downto 1)=I2C_ADR;
                      end if;
                      if DEBUG then
                         printf("state=idle acc_done=%d",acc_done);
                         printf(" my_adr=%d\n",my_adr);
                      end if;
                      if acc_done and my_adr then
                         state <= slave_ack;
                         rw    <= sr(0);
                         sda_o <= '0'; -- generate i2c_ack

                         if DEBUG then
                            assert false report "Command byte received" severity note;
                            if sr(0)='1' then
                               printf("\n\nRead command 0x%X\n\n",sr);
                            else
                               printf("\n\nWrite command 0x%X\n\n",sr);
                            end if;
                         end if;

                         if sr(0)='1' then
                            --Wait for 1 us;
                            mem_do <= mem(to_integer(mem_adr));
                            if DEBUG then
                               assert false report "Data block read" severity note;
                            end if;
                         end if;
                      end if;

                 when slave_ack =>
                      if DEBUG then
                         printf("state=slave_ack\n");
                      end if;
                      if rw='1' then
                         --Wait for 1 us;
                         state <= data;
                         ld    <= '1';
                         sda_o <= mem_do(7);
                         mem_do<= mem_do(6 downto 0) & '1'; -- insert '1' for host ack generation
                      else
                         --Wait for 1 us;
                         state <= get_mem_adr;
                         ld    <= '1';
                      end if;

                 when get_mem_adr => -- wait for memory address
                      if DEBUG then
                         printf("state=get_mem_adr\n");
                      end if;
                      if acc_done then
                         if unsigned(sr)<=15 then -- generate i2c_ack, for valid address
                            --Wait for 1 us;
                            sda_o <= '0';
                         else
                            --Wait for 1 us;
                            sda_o <= '1';
                         end if;
                         state   <= gma_ack;
                         mem_adr <= unsigned(sr(1 downto 0)); -- store memory address

                         if DEBUG then
                            assert false report "Address received" severity note;
                            printf("\nMemory Address 0x%X (next state is gma_ack)\n\n",sr);
                         end if;
                      end if;

                 when gma_ack =>
                      if DEBUG then
                         printf("state=gma_ack\n");
                      end if;
                      --Wait for 1 us;
                      state <= data;
                      ld    <= '1';

                 when data => -- receive or drive data
                      if DEBUG then
                         printf("state=data bit_cnt=%d",to_integer(bit_cnt));
                         printf(" acc_done=%d\n",acc_done);
                      end if;
                      if rw='1' then
                         --Wait for 1 us;
                         sda_o  <= mem_do(7);
                         mem_do <= mem_do(6 downto 0) & '1';    -- insert '1' for host ack generation
                      end if;

                      if acc_done then
                         state <= data_ack;
                         if rw='0' and (mem_adr<=15) then
                            sda_o   <= '0'; -- send ack on write
                         end if;

                         --Wait for 1 us;
                         mem_adr <= mem_adr+1;

                         if rw='1' then
                            --Wait for 3 us;
                            mem_do <= mem(to_integer(mem_adr+1));
                            if DEBUG then
                               assert false report "Data block read" severity note;
                               printf("\nEnd of read address 0x%X=",to_integer(mem_adr));
                               printf("0x%X\n\n",mem(to_integer(mem_adr)));
                            end if;
                         else
                            --Wait for 1 us;
                            mem(to_integer(mem_adr)) <= sr; -- store data in memory
                            if DEBUG then
                               assert false report "Data block write" severity note;
                               printf("\nEnd of write 0x%X ",sr);
                               printf("to 0x%X\n\n",to_integer(mem_adr));
                            end if;
                         end if;
                      end if;

                 when data_ack =>
                      if DEBUG then
                         printf("state=data_ack\n");
                      end if;
                      --Wait for 1 us;
                      ld <= '1';

                      if rw='1' then
                         if sda='1' then -- read operation && master send NACK
                            state <= idle;
                            sda_o <= '1';
                         else
                            state <= data;
                            sda_o <= mem_do(7);
                            mem_do<= mem_do(6 downto 0) & '1';    -- insert '1' for host ack generation
                         end if;
                      else
                         state <= data;
                         sda_o <= '1';
                      end if;
            end case;
         end if;
      end if;
   end process Statemachine;

   -- Generate tri-states
   sda_x_io<= 'Z' when sda_o='1' else '0';

   -- Solve the "H" state
   sda <= to_x01z(sda_x_io);
   scl <= to_x01z(scl_x_i);
end architecture Simulator; -- of entity I2C_Memory
