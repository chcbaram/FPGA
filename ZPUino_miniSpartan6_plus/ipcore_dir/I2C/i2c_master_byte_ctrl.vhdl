------------------------------------------------------------------------------
----                                                                      ----
----  I2C Master Core (Byte Controller)                                   ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----  Based on code from: http://www.opencores.org/projects/i2c/          ----
----                                                                      ----
----  Description:                                                        ----
----  I2C master peripheral for the Wishbone bus.                         ----
----  Byte controller stuff. That's almost the same code from OpenCores.  ----
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
---- Design unit:      I2C_MasterByteCtrl(Structural) (Entity and arch.)  ----
---- File name:        i2c_master_byte_ctrl.vhdl                          ----
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
--  $Id: i2c_master_byte_ctrl.vhdl,v 1.8 2006/04/17 19:44:43 salvador Exp $
--
--  $Date: 2006/04/17 19:44:43 $
--  $Revision: 1.8 $
--  $Author: salvador $
--  $Locker:  $
--  $State: Exp $
--
-- Change History:
--               $Log: i2c_master_byte_ctrl.vhdl,v $
--               Revision 1.8  2006/04/17 19:44:43  salvador
--               * Modified: License to GPL.
--
--               Revision 1.7  2005/05/20 14:39:05  salvador
--               * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
--               Revision 1.6  2005/05/18 14:50:19  salvador
--               * Modificado: Los encabezados de los archivos para que cumplan con nuestras
--               recomendaciones.
--
--               Revision 1.5  2005/05/11 22:39:17  salvador
--               * Modificado: Pasado por el bakalint 0.3.5.
--
--               Revision 1.4  2005/03/29 20:35:44  salvador
--               * Modificado: Encerrado con "translate off/on" el código de simulación para
--               que el XST no moleste.
--               * Agregado: Un generic para que las interrupciones esten siempre
--               habilitadas.
--               * Agregado: Default para wb_cyc_i de manera tal que no sea necesario
--               conectarlo.
--               * Modificado: Para ahorrar algunos F/F en registros que tienen bits sin
--               usar. A consecuencia de esto el bit "iack" se corrió.
--
--               Revision 1.3  2005/03/10 19:40:07  salvador
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
--               Revision 1.2  2005/03/09 17:41:16  salvador
--               * Agregado: Hojas de datos del 24LC02B.
--               * Modificado: Reemplazo de Report por Assert porque las herramientas de
--               Xilinx no lo soportan.
--               * Modificado: Comentado los printf en core porque no tengo el equivalente
--               para Xilinx.
--               * Corregido: El TB de la memoria no contestaba ACK luego de la escritura.
--               Ahora si y además el TB verifica que no falten ACKs.
--
--               Revision 1.1  2005/03/08 15:57:36  salvador
--               * Movido al repositorio CVS.
--               * Agregado: TestBench en VHDL.
--
--               Revision 1.5  2004/02/18 11:41:48  rherveille
--               Fixed a potential bug in the statemachine. During a 'stop' 2 cmd_ack signals were generated. Possibly canceling a new start command.
--
--               Revision 1.4  2003/08/09 07:01:13  rherveille
--               Fixed a bug in the Arbitration Lost generation caused by delay on the (external) sda line.
--               Fixed a potential bug in the byte controller's host-acknowledge generation.
--
--               Revision 1.3  2002/12/26 16:05:47  rherveille
--               Core is now a Multimaster I2C controller.
--
--               Revision 1.2  2002/11/30 22:24:37  rherveille
--               Cleaned up code
--
--               Revision 1.1  2001/11/05 12:02:33  rherveille
--               Split i2c_master_core.vhd into separate files for each entity; same layout as verilog version.
--               Code updated, is now up-to-date to doc. rev.0.4.
--               Added headers.
--




--
------------------------------------------
-- Byte controller section
------------------------------------------
--
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity I2C_MasterByteCtrl is
   port (
      wb_clk_i        : in std_logic;
      wb_rst_i        : in std_logic; -- synchronous active high reset (WISHBONE compatible)
      nreset_i        : in std_logic;  -- asynchornous active low reset (FPGA compatible)
      ena_i           : in std_logic; -- core enable signal

      clk_cnt_i       : in unsigned(15 downto 0);     -- 4x SCL

      -- input signals
      start_i         : in std_logic;
      stop_i          : in std_logic;
      read_i          : in std_logic;
      write_i         : in std_logic;
      ack_in_i        : in std_logic;
      din_i           : in std_logic_vector(7 downto 0);

      -- output signals
      cmd_ack_o       : out std_logic; -- command done
      ack_out_o       : out std_logic;
      i2c_busy_o      : out std_logic; -- arbitration lost
      i2c_al_o        : out std_logic; -- i2c bus busy
      dout_o          : out std_logic_vector(7 downto 0);

      -- i2c lines
      scl_i           : in  std_logic;  -- i2c clock line input
      scl_o           : out std_logic; -- i2c clock line output
      scl_oen_o       : out std_logic; -- i2c clock line output enable, active low
      sda_i           : in  std_logic;  -- i2c data line input
      sda_o           : out std_logic; -- i2c data line output
      sda_oen_o       : out std_logic  -- i2c data line output enable, active low
      );
end entity I2C_MasterByteCtrl;

architecture Structural of I2C_MasterByteCtrl is
   component I2C_MasterBitCtrl is
      port (
         wb_clk_i        : in std_logic;
         wb_rst_i        : in std_logic;
         nreset_i        : in std_logic;
         ena_i           : in std_logic;                         -- core enable signal

         clk_cnt_i       : in unsigned(15 downto 0);             -- clock prescale value

         cmd_i           : in std_logic_vector(3 downto 0);
         cmd_ack_o       : out std_logic; -- command done
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
   end component I2C_MasterBitCtrl;

   -- commands for bit_controller block
   constant I2C_CMD_NOP   : std_logic_vector(3 downto 0) := "0000";
   constant I2C_CMD_START : std_logic_vector(3 downto 0) := "0001";
   constant I2C_CMD_STOP  : std_logic_vector(3 downto 0) := "0010";
   constant I2C_CMD_READ  : std_logic_vector(3 downto 0) := "0100";
   constant I2C_CMD_WRITE : std_logic_vector(3 downto 0) := "1000";

   -- signals for bit_controller
   signal core_cmd        : std_logic_vector(3 downto 0);
   signal core_ack        : std_logic;
   signal core_txd        : std_logic;
   signal core_rxd        : std_logic;
   signal al_o            : std_logic;

   -- signals for shift register
   signal sr              : std_logic_vector(7 downto 0); -- 8bit shift register
   signal shift, ld       : std_logic;

   -- signals for state machine
   signal go, host_ack    : std_logic;
   signal dcnt            : unsigned(2 downto 0); -- data counter
   signal cnt_done        : std_logic;

begin
   -- hookup bit_controller
   bit_ctrl: I2C_MasterBitCtrl
      port map(
         wb_clk_i=> wb_clk_i,
         wb_rst_i=> wb_rst_i,
         nreset_i=> nreset_i,
         ena_i   => ena_i,
         clk_cnt_i=> clk_cnt_i,
         cmd_i   => core_cmd,
         cmd_ack_o=> core_ack,
         busy_o  => i2c_busy_o,
         al_o    => al_o,
         din_i   => core_txd,
         dout_o  => core_rxd,
         scl_i   => scl_i,
         scl_o   => scl_o,
         scl_oen_o=> scl_oen_o,
         sda_i   => sda_i,
         sda_o   => sda_o,
         sda_oen_o=> sda_oen_o      );
   i2c_al_o<= al_o;

   -- generate host-command-acknowledge
   cmd_ack_o<= host_ack;

   -- generate go-signal
   go <= (read_i or write_i or stop_i) and not host_ack;

   -- assign Dout output to shift-register
   dout_o<= sr;

   -- generate shift register
   shift_register:
   process(wb_clk_i, nreset_i)
   begin
      if (nreset_i= '0') then
         sr <= (others => '0');
      elsif rising_edge(wb_clk_i) then
         if (wb_rst_i= '1') then
            sr <= (others => '0');
         elsif (ld = '1') then
            sr <= din_i;
         elsif (shift = '1') then
            sr <= (sr(6 downto 0) & core_rxd);
         end if;
      end if;
   end process shift_register;

   -- generate data-counter
   data_cnt:
   process(wb_clk_i, nreset_i)
   begin
      if (nreset_i= '0') then
         dcnt <= (others => '0');
      elsif rising_edge(wb_clk_i) then
         if (wb_rst_i= '1') then
            dcnt <= (others => '0');
         elsif (ld = '1') then
            dcnt <= (others => '1');  -- load counter with 7
         elsif (shift = '1') then
            dcnt <= dcnt -1;
         end if;
      end if;
   end process data_cnt;

   cnt_done <= '1' when (dcnt = 0) else '0';

   --
   -- state machine
   --
   statemachine:
   block
      type states is (st_idle, st_start, st_read, st_write, st_ack, st_stop);
      signal c_state         : states;
   begin
      --
      -- command interpreter, translate complex commands into simpler I2C commands
      --
      nxt_state_decoder:
      process(wb_clk_i, nreset_i)
      begin
         if (nreset_i= '0') then
            core_cmd <= I2C_CMD_NOP;
            core_txd <= '0';
            shift    <= '0';
            ld       <= '0';
            host_ack <= '0';
            c_state  <= st_idle;
            ack_out_o<= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i= '1' or al_o= '1') then
               core_cmd <= I2C_CMD_NOP;
               core_txd <= '0';
               shift    <= '0';
               ld       <= '0';
               host_ack <= '0';
               c_state  <= st_idle;
               ack_out_o<= '0';
            else
               -- initialy reset all signal
               core_txd <= sr(7);
               shift    <= '0';
               ld       <= '0';
               host_ack <= '0';

               case c_state is
                    when st_idle =>
                         if (go = '1') then
                            if (start_i= '1') then
                               c_state  <= st_start;
                               core_cmd <= I2C_CMD_START;
                            elsif (read_i= '1') then
                               c_state  <= st_read;
                               core_cmd <= I2C_CMD_READ;
                            elsif (write_i= '1') then
                               c_state  <= st_write;
                               core_cmd <= I2C_CMD_WRITE;
                            else -- stop
                               c_state  <= st_stop;
                               core_cmd <= I2C_CMD_STOP;
                            end if;

                            ld <= '1';
                         end if;

                    when st_start =>
                         if (core_ack = '1') then
                            if (read_i= '1') then
                               c_state  <= st_read;
                               core_cmd <= I2C_CMD_READ;
                            else
                               c_state  <= st_write;
                               core_cmd <= I2C_CMD_WRITE;
                            end if;

                            ld <= '1';
                         end if;

                    when st_write =>
                         if (core_ack = '1') then
                            if (cnt_done = '1') then
                               c_state  <= st_ack;
                               core_cmd <= I2C_CMD_READ;
                            else
                               c_state  <= st_write;       -- stay in same state
                               core_cmd <= I2C_CMD_WRITE;  -- write next bit
                               shift    <= '1';
                            end if;
                         end if;

                    when st_read =>
                         if (core_ack = '1') then
                            if (cnt_done = '1') then
                               c_state  <= st_ack;
                               core_cmd <= I2C_CMD_WRITE;
                            else
                               c_state  <= st_read;      -- stay in same state
                               core_cmd <= I2C_CMD_READ; -- read next bit
                            end if;

                            shift    <= '1';
                            core_txd <= ack_in_i;
                         end if;

                    when st_ack =>
                         if (core_ack = '1') then
                            -- check for stop; Should a STOP command be generated ?
                            if (stop_i= '1') then
                               c_state  <= st_stop;
                               core_cmd <= I2C_CMD_STOP;
                            else
                               c_state  <= st_idle;
                               core_cmd <= I2C_CMD_NOP;

                               -- generate command acknowledge signal
                               host_ack <= '1';
                            end if;

                            -- assign ack_out output to core_rxd (contains last received bit)
                            ack_out_o<= core_rxd;

                            core_txd <= '1';
                         else
                            core_txd <= ack_in_i;
                         end if;

                    when st_stop =>
                         if (core_ack = '1') then
                            c_state  <= st_idle;
                            core_cmd <= I2C_CMD_NOP;

                            -- generate command acknowledge signal
                            host_ack <= '1';
                         end if;

                    when others => -- illegal states
                         c_state  <= st_idle;
                         core_cmd <= I2C_CMD_NOP;
                         --synopsys translate off
                         assert false
                            report "Byte controller entered illegal state."
                            severity failure;
                         --synopsys translate on

               end case;

            end if;
         end if;
      end process nxt_state_decoder;

   end block statemachine;

end architecture Structural;

