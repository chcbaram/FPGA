------------------------------------------------------------------------------
----                                                                      ----
----  I2C Master Core (Bit Controller)                                    ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----  Based on code from: http://www.opencores.org/projects/i2c/          ----
----                                                                      ----
----  Description:                                                        ----
----  I2C master peripheral for the Wishbone bus.                         ----
----  Bit controller stuff. That's almost the same code from OpenCores.   ----
----                                                                      ----
----  To Do:                                                              ----
----  -                                                                   ----
----                                                                      ----
----  Authors:                                                            ----
----    - Richard Herveille, richard@asics.ws                             ----
----    - Salvador E. Tropea, salvador en inti gov ar (small changes)     ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Copyright (c) 2005 Salvador E. Tropea <salvador en inti gov ar>      ----
---- Copyright (c) 2005 Instituto Nacional de Tecnología Industrial       ----
---- Copyright (c) 2000 Richard Herveille  <richard@asics.ws>             ----
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
---- Design unit:      I2C_MasterBitCtrl(Structural) (Entity and arch.)   ----
---- File name:        i2c_master_bit_ctrl.vhdl                           ----
---- Note:             None                                               ----
---- Limitations:      None known                                         ----
---- Errors:           None known                                         ----
---- Library:          i2c_mwb                                            ----
---- Dependencies:     IEEE.std_logic_1164                                ----
----                   IEEE.numeric_std                                   ----
---- Target FPGA:      Spartan II (XC2S100-5-PQ208)                       ----
---- Language:         VHDL                                               ----
---- Wishbone:         None                                               ----
---- Synthesis tools:  Xilinx Release 6.2.03i - xst G.31a                 ----
---- Simulation tools: GHDL [Sokcho edition] (0.1x)                       ----
---- Text editor:      SETEdit 0.5.x                                      ----
----                                                                      ----
------------------------------------------------------------------------------
--
--  CVS Log
--
--  $Id: i2c_master_bit_ctrl.vhdl,v 1.6 2006/04/17 19:44:43 salvador Exp $
--
--  $Date: 2006/04/17 19:44:43 $
--  $Revision: 1.6 $
--  $Author: salvador $
--  $Locker:  $
--  $State: Exp $
--
-- Change History:
--               $Log: i2c_master_bit_ctrl.vhdl,v $
--               Revision 1.6  2006/04/17 19:44:43  salvador
--               * Modified: License to GPL.
--
--               Revision 1.5  2005/05/20 14:39:05  salvador
--               * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
--               Revision 1.4  2005/05/18 14:50:19  salvador
--               * Modificado: Los encabezados de los archivos para que cumplan con nuestras
--               recomendaciones.
--
--               Revision 1.3  2005/05/11 22:39:17  salvador
--               * Modificado: Pasado por el bakalint 0.3.5.
--
--               Revision 1.2  2005/03/10 19:40:07  salvador
--               * Modificado: Para usar "rising_edge" que hace más legible el código.
--               * Agregado: MUX_BETTER para elegir que use muxs en lugar de tri-states.
--               Por defecto es falso con lo que ahorra unos 12 slice.
--               * Agregado: FULL_SYNC para lograr el comportamiento original con 1 WS.
--               * Agregado: FIXED_PRER con lo que se puede fijar el valor del prescaler lo
--               que ahorra unos 11 slice.
--               * Modificado: Los case de lectura/escritura de los registros por if/elsif
--               que permite controlar mejor el uso de los generic.
--               * Modificado: El testbench para que soporte FIXED_PRER.
--
--               Revision 1.1  2005/03/08 15:57:36  salvador
--               * Movido al repositorio CVS.
--               * Agregado: TestBench en VHDL.
--
--               Revision 1.12  2004/05/07 11:53:31  rherveille
--               Fixed previous fix :) Made a variable vs signal mistake.
--
--               Revision 1.11  2004/05/07 11:04:00  rherveille
--               Fixed a bug where the core would signal an arbitration lost (AL bit set), when another master controls the bus and the other master generates a STOP bit.
--
--               Revision 1.10  2004/02/27 07:49:43  rherveille
--               Fixed a bug in the arbitration-lost signal generation. VHDL version only.
--
--               Revision 1.9  2003/08/12 14:48:37  rherveille
--               Forgot an 'end if' :-/
--
--               Revision 1.8  2003/08/09 07:01:13  rherveille
--               Fixed a bug in the Arbitration Lost generation caused by delay on the (external) sda line.
--               Fixed a potential bug in the byte controller's host-acknowledge generation.
--
--               Revision 1.7  2003/02/05 00:06:02  rherveille
--               Fixed a bug where the core would trigger an erroneous 'arbitration lost' interrupt after being reset, when the reset pulse width < 3 clk cycles.
--
--               Revision 1.6  2003/02/01 02:03:06  rherveille
--               Fixed a few 'arbitration lost' bugs. VHDL version only.
--
--               Revision 1.5  2002/12/26 16:05:47  rherveille
--               Core is now a Multimaster I2C controller.
--
--               Revision 1.4  2002/11/30 22:24:37  rherveille
--               Cleaned up code
--
--               Revision 1.3  2002/10/30 18:09:53  rherveille
--               Fixed some reported minor start/stop generation timing issuess.
--
--               Revision 1.2  2002/06/15 07:37:04  rherveille
--               Fixed a small timing bug in the bit controller.\nAdded verilog simulation environment.
--
--               Revision 1.1  2001/11/05 12:02:33  rherveille
--               Split i2c_master_core.vhd into separate files for each entity; same layout as verilog version.
--               Code updated, is now up-to-date to doc. rev.0.4.
--               Added headers.
--


--
-------------------------------------
-- Bit controller section
------------------------------------
--
-- Translate simple commands into SCL/SDA transitions
-- Each command has 5 states, A/B/C/D/idle
--
-- start:    SCL  ~~~~~~~~~~~~~~\____
--           SDA  XX/~~~~~~~\______
--                x | A | B | C | D | i
--
-- repstart  SCL  ______/~~~~~~~\___
--           SDA  __/~~~~~~~\______
--                x | A | B | C | D | i
--
-- stop      SCL  _______/~~~~~~~~~~~
--           SDA  ==\___________/~~~~~
--                x | A | B | C | D | i
--
--- write    SCL  ______/~~~~~~~\____
--           SDA  XXX===============XX
--                x | A | B | C | D | i
--
--- read     SCL  ______/~~~~~~~\____
--           SDA  XXXXXXX=XXXXXXXXXXX
--                x | A | B | C | D | i
--

-- Timing:      Normal mode     Fast mode
-----------------------------------------------------------------
-- Fscl         100KHz          400KHz
-- Th_scl       4.0us           0.6us   High period of SCL
-- Tl_scl       4.7us           1.3us   Low period of SCL
-- Tsu:sta      4.7us           0.6us   setup time for a repeated start condition
-- Tsu:sto      4.0us           0.6us   setup time for a stop conditon
-- Tbuf         4.7us           1.3us   Bus free time between a stop and start condition
--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity I2C_MasterBitCtrl is
   port(
      wb_clk_i        : in std_logic;
      wb_rst_i        : in std_logic;
      nreset_i        : in std_logic;
      ena_i           : in std_logic;             -- core enable signal

      clk_cnt_i       : in unsigned(15 downto 0); -- clock prescale value

      cmd_i           : in std_logic_vector(3 downto 0);
      cmd_ack_o       : out std_logic; -- command completed
      busy_o          : out std_logic; -- i2c bus busy
      al_o            : out std_logic; -- arbitration lost

      din_i           : in std_logic;
      dout_o          : out std_logic;

      -- i2c lines
      scl_i           : in std_logic;  -- i2c clock line input
      scl_o           : out std_logic; -- i2c clock line output
      scl_oen_o       : out std_logic; -- i2c clock line output enable, active low
      sda_i           : in std_logic;  -- i2c data line input
      sda_o           : out std_logic; -- i2c data line output
      sda_oen_o       : out std_logic  -- i2c data line output enable, active low
      );
end entity I2C_MasterBitCtrl;

architecture Structural of I2C_MasterBitCtrl is
   constant I2C_CMD_NOP   : std_logic_vector(3 downto 0) := "0000";
   constant I2C_CMD_START : std_logic_vector(3 downto 0) := "0001";
   constant I2C_CMD_STOP  : std_logic_vector(3 downto 0) := "0010";
   constant I2C_CMD_READ  : std_logic_vector(3 downto 0) := "0100";
   constant I2C_CMD_WRITE : std_logic_vector(3 downto 0) := "1000";

   type states is (idle, start_a, start_b, start_c, start_d, start_e,
   stop_a, stop_b, stop_c, stop_d, rd_a, rd_b, rd_c,
   rd_d, wr_a, wr_b, wr_c, wr_d);
   signal c_state         : states;

   signal iscl_oen        : std_logic;          -- internal I2C lines
   signal isda_oen        : std_logic;          -- internal I2C lines
   signal sda_chk         : std_logic;          -- check SDA status (multi-master arbitration)
   signal dscl_oen        : std_logic;          -- delayed scl_oen signals
   signal sscl            : std_logic;          -- synchronized SCL inputs
   signal ssda            : std_logic;          -- synchronized SDA inputs
   signal clk_en          : std_logic;          -- clock generation signals
   signal slave_wait      : std_logic;          -- clock generation signals
   signal ial             : std_logic;          -- internal arbitration lost signal
   --      signal cnt : unsigned(15 downto 0) := clk_cnt;  -- clock divider counter (simulation)
   signal cnt             : unsigned(15 downto 0); -- clock divider counter (synthesis)
begin
   -- whenever the slave is not ready it can delay the cycle by pulling SCL low
   -- delay scl_oen
   delay_scl_oen:
   process (wb_clk_i)
   begin
      if rising_edge(wb_clk_i) then
         dscl_oen <= iscl_oen;
      end if;
   end process delay_scl_oen;
   slave_wait <= dscl_oen and not sscl;

   -- generate clk enable signal
   gen_clken:
   process(wb_clk_i, nreset_i)
   begin
      if (nreset_i= '0') then
         cnt    <= (others => '0');
         clk_en <= '1';
      elsif rising_edge(wb_clk_i) then
         if (wb_rst_i= '1') then
            cnt    <= (others => '0');
            clk_en <= '1';
         else
            if ((cnt = 0) or (ena_i= '0')) then
               if (slave_wait = '0') then
                  cnt    <= clk_cnt_i;
                  clk_en <= '1';
               else
                  cnt    <= cnt;
                  clk_en <= '0';
               end if;
            else
               if (slave_wait = '0') then
                  cnt <= cnt -1;
               end if;
               clk_en <= '0';
            end if;
         end if;
      end if;
   end process gen_clken;


   -- generate bus status controller
   bus_status_ctrl:
   block
      signal dscl, dsda      : std_logic;  -- delayes sSCL and sSDA
      signal sta_condition   : std_logic;  -- start detected
      signal sto_condition   : std_logic;  -- stop detected
      signal cmd_stop        : std_logic;  -- STOP command
      signal ibusy           : std_logic;  -- internal busy signal
   begin
      -- synchronize SCL and SDA inputs
      synch_scl_sda:
      process(wb_clk_i, nreset_i)
      begin
         if (nreset_i= '0') then
            sscl <= '1';
            ssda <= '1';

            dscl <= '1';
            dsda <= '1';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i= '1') then
               sscl <= '1';
               ssda <= '1';

               dscl <= '1';
               dsda <= '1';
            else
               sscl <= scl_i;
               ssda <= sda_i;

               dscl <= sscl;
               dsda <= ssda;
            end if;
         end if;
      end process synch_scl_sda;

      -- detect start condition => detect falling edge on SDA while SCL is high
      -- detect stop condition  => detect rising edge on SDA while SCL is high
      detect_sta_sto:
      process(wb_clk_i, nreset_i)
      begin
         if (nreset_i= '0') then
            sta_condition <= '0';
            sto_condition <= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i= '1') then
               sta_condition <= '0';
               sto_condition <= '0';
            else
               sta_condition <= (not ssda and dsda) and sscl;
               sto_condition <= (ssda and not dsda) and sscl;
            end if;
         end if;
      end process detect_sta_sto;

      -- generate i2c-bus busy signal
      gen_busy:
      process(wb_clk_i, nreset_i)
      begin
         if (nreset_i= '0') then
            ibusy <= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i= '1') then
               ibusy <= '0';
            else
               ibusy <= (sta_condition or ibusy) and not sto_condition;
            end if;
         end if;
      end process gen_busy;
      busy_o<= ibusy;


      -- generate arbitration lost signal
      -- aribitration lost when:
      -- 1) master drives SDA high, but the i2c bus is low
      -- 2) stop detected while not requested (detect during 'idle' state)
      gen_al:
      process(wb_clk_i, nreset_i)
      begin
         if (nreset_i= '0') then
            cmd_stop  <= '0';
            ial       <= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i= '1') then
               cmd_stop  <= '0';
               ial       <= '0';
            else
               if (clk_en = '1') then
                  if (cmd_i= I2C_CMD_STOP) then
                     cmd_stop <= '1';
                  else
                     cmd_stop <= '0';
                  end if;
               end if;

               if (c_state = idle) then
                  ial <= (sda_chk and not ssda and isda_oen);
               else
                  ial <= (sda_chk and not ssda and isda_oen) or (sto_condition and not cmd_stop);
               end if;

            end if;
         end if;
      end process gen_al;
      al_o<= ial;

      -- generate dout signal, store dout on rising edge of SCL
      gen_dout:
      process(wb_clk_i)
      begin
         if rising_edge(wb_clk_i) then
            if (sscl = '1' and dscl = '0') then
               dout_o<= ssda;
            end if;
         end if;
      end process gen_dout;
   end block bus_status_ctrl;


   -- generate statemachine
   nxt_state_decoder:
   process (wb_clk_i, nreset_i, c_state, cmd_i)
   begin
      if (nreset_i= '0') then
         c_state  <= idle;
         cmd_ack_o<= '0';
         iscl_oen <= '1';
         isda_oen <= '1';
         sda_chk  <= '0';
      elsif rising_edge(wb_clk_i) then
         if (wb_rst_i= '1' or ial = '1') then
            c_state  <= idle;
            cmd_ack_o<= '0';
            iscl_oen <= '1';
            isda_oen <= '1';
            sda_chk  <= '0';
         else
            cmd_ack_o<= '0'; -- default no acknowledge

            if (clk_en = '1') then
               case (c_state) is
                         -- idle
                    when idle =>
                         case cmd_i is
                              when I2C_CMD_START => c_state <= start_a;
                              when I2C_CMD_STOP  => c_state <= stop_a;
                              when I2C_CMD_WRITE => c_state <= wr_a;
                              when I2C_CMD_READ  => c_state <= rd_a;
                              when others        => c_state <= idle; -- NOP command
                         end case;

                         iscl_oen <= iscl_oen; -- keep SCL in same state
                         isda_oen <= isda_oen; -- keep SDA in same state
                         sda_chk  <= '0';      -- don't check SDA

                         -- start
                    when start_a =>
                         c_state  <= start_b;
                         iscl_oen <= iscl_oen; -- keep SCL in same state (for repeated start)
                         isda_oen <= '1';      -- set SDA high
                         sda_chk  <= '0';      -- don't check SDA

                    when start_b =>
                         c_state  <= start_c;
                         iscl_oen <= '1'; -- set SCL high
                         isda_oen <= '1'; -- keep SDA high
                         sda_chk  <= '0'; -- don't check SDA

                    when start_c =>
                         c_state  <= start_d;
                         iscl_oen <= '1'; -- keep SCL high
                         isda_oen <= '0'; -- set SDA low
                         sda_chk  <= '0'; -- don't check SDA

                    when start_d =>
                         c_state  <= start_e;
                         iscl_oen <= '1'; -- keep SCL high
                         isda_oen <= '0'; -- keep SDA low
                         sda_chk  <= '0'; -- don't check SDA

                    when start_e =>
                         c_state  <= idle;
                         cmd_ack_o<= '1'; -- command completed
                         iscl_oen <= '0'; -- set SCL low
                         isda_oen <= '0'; -- keep SDA low
                         sda_chk  <= '0'; -- don't check SDA

                         -- stop
                    when stop_a =>
                         c_state  <= stop_b;
                         iscl_oen <= '0'; -- keep SCL low
                         isda_oen <= '0'; -- set SDA low
                         sda_chk  <= '0'; -- don't check SDA

                    when stop_b =>
                         c_state  <= stop_c;
                         iscl_oen <= '1'; -- set SCL high
                         isda_oen <= '0'; -- keep SDA low
                         sda_chk  <= '0'; -- don't check SDA

                    when stop_c =>
                         c_state  <= stop_d;
                         iscl_oen <= '1'; -- keep SCL high
                         isda_oen <= '0'; -- keep SDA low
                         sda_chk  <= '0'; -- don't check SDA

                    when stop_d =>
                         c_state  <= idle;
                         cmd_ack_o<= '1'; -- command completed
                         iscl_oen <= '1'; -- keep SCL high
                         isda_oen <= '1'; -- set SDA high
                         sda_chk  <= '0'; -- don't check SDA

                         -- read
                    when rd_a =>
                         c_state  <= rd_b;
                         iscl_oen <= '0'; -- keep SCL low
                         isda_oen <= '1'; -- tri-state SDA
                         sda_chk  <= '0'; -- don't check SDA

                    when rd_b =>
                         c_state  <= rd_c;
                         iscl_oen <= '1'; -- set SCL high
                         isda_oen <= '1'; -- tri-state SDA
                         sda_chk  <= '0'; -- don't check SDA

                    when rd_c =>
                         c_state  <= rd_d;
                         iscl_oen <= '1'; -- keep SCL high
                         isda_oen <= '1'; -- tri-state SDA
                         sda_chk  <= '0'; -- don't check SDA

                    when rd_d =>
                         c_state  <= idle;
                         cmd_ack_o<= '1'; -- command completed
                         iscl_oen <= '0'; -- set SCL low
                         isda_oen <= '1'; -- tri-state SDA
                         sda_chk  <= '0'; -- don't check SDA

                         -- write
                    when wr_a =>
                         c_state  <= wr_b;
                         iscl_oen <= '0'; -- keep SCL low
                         isda_oen <= din_i; -- set SDA
                         sda_chk  <= '0'; -- don't check SDA (SCL low)

                    when wr_b =>
                         c_state  <= wr_c;
                         iscl_oen <= '1'; -- set SCL high
                         isda_oen <= din_i; -- keep SDA
                         sda_chk  <= '1'; -- check SDA

                    when wr_c =>
                         c_state  <= wr_d;
                         iscl_oen <= '1'; -- keep SCL high
                         isda_oen <= din_i; -- keep SDA
                         sda_chk  <= '1'; -- check SDA

                    when wr_d =>
                         c_state  <= idle;
                         cmd_ack_o<= '1'; -- command completed
                         iscl_oen <= '0'; -- set SCL low
                         isda_oen <= din_i; -- keep SDA
                         sda_chk  <= '0'; -- don't check SDA (SCL low)

                    when others =>

               end case;
            end if;
         end if;
      end if;
   end process nxt_state_decoder;


   -- assign outputs
   scl_o   <= '0';
   scl_oen_o<= iscl_oen;
   sda_o   <= '0';
   sda_oen_o<= isda_oen;
end architecture Structural;

