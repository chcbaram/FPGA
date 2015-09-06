------------------------------------------------------------------------------
----                                                                      ----
----  I2C Master Core (Top Level)                                         ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----  Based on code from: http://www.opencores.org/projects/i2c/          ----
----                                                                      ----
----  Description:                                                        ----
----  I2C master peripheral for the Wishbone bus.                         ----
----  Top level of the core.                                              ----
----  I added various generics to customize the core:                     ----
----  * DEBUG      enable debug registers                                 ----
----  * MUX_BETTER true if using MUX is better than using tri-states      ----
----  * FULL_SYNC  true if you need full synchronous behavior,            ----
----               introduces 1 WS                                        ----
----  * FIXED_PRER assigning a value removes the PRER and uses it as      ----
----               pre-scaler                                             ----
----  * USE_IEN    false if interrupts are always enabled (masked in      ----
----               another component)                                     ----
----                                                                      ----
----  To Do:                                                              ----
----  -                                                                   ----
----                                                                      ----
----  Authors:                                                            ----
----    - Richard Herveille, richard@asics.ws                             ----
----    - Salvador E. Tropea, salvador en inti gov ar (additions & optim.)----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Copyright (c) 2005 Salvador E. Tropea <salvador en inti gov ar>      ----
---- Copyright (c) 2005 Instituto Nacional de Tecnolog Industrial       ----
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
---- Design unit:      I2C_MasterTop(Structural) (Entity and architecture)----
---- File name:        i2c_master_top.vhdl                                ----
---- Note:             None                                               ----
---- Limitations:      None known                                         ----
---- Errors:           None known                                         ----
---- Library:          i2c_mwb                                            ----
---- Dependencies:     IEEE.std_logic_1164                                ----
----                   IEEE.numeric_std                                   ----
----                   c.stdio_h                                          ----
---- Target FPGA:      Spartan II (XC2S100-5-PQ208)                       ----
---- Language:         VHDL                                               ----
---- Wishbone:         SLAVE (rev B.2)                                    ----
---- Synthesis tools:  Xilinx Release 6.2.03i - xst G.31a                 ----
---- Simulation tools: GHDL [Sokcho edition] (0.1x)                       ----
---- Text editor:      SETEdit 0.5.x                                      ----
----                                                                      ----
------------------------------------------------------------------------------
--
--  CVS Log
--
--  $Id: i2c_master_top.vhdl,v 1.12 2006/04/18 13:37:08 salvador Exp $
--
--  $Date: 2006/04/18 13:37:08 $
--  $Revision: 1.12 $
--  $Author: salvador $
--  $Locker:  $
--  $State: Exp $
--
-- Change History:
--               $Log: i2c_master_top.vhdl,v $
--               Revision 1.12  2006/04/18 13:37:08  salvador
--               * Modificado: Peques retoques al indentado.
--
--               Revision 1.11  2006/04/17 19:44:43  salvador
--               * Modified: License to GPL.
--
--               Revision 1.10  2005/05/20 14:39:05  salvador
--               * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
--               Revision 1.9  2005/05/18 14:50:19  salvador
--               * Modificado: Los encabezados de los archivos para que cumplan con nuestras
--               recomendaciones.
--
--               Revision 1.8  2005/05/11 22:39:18  salvador
--               * Modificado: Pasado por el bakalint 0.3.5.
--
--               Revision 1.7  2005/03/29 20:35:44  salvador
--               * Modificado: Encerrado con "translate off/on" el cigo de simulaci para
--               que el XST no moleste.
--               * Agregado: Un generic para que las interrupciones esten siempre
--               habilitadas.
--               * Agregado: Default para wb_cyc_i de manera tal que no sea necesario
--               conectarlo.
--               * Modificado: Para ahorrar algunos F/F en registros que tienen bits sin
--               usar. A consecuencia de esto el bit "iack" se corri
--
--               Revision 1.6  2005/03/10 19:40:07  salvador
--               * Modificado: Para usar "rising_edge" que hace m legible el cigo.
--               * Agregado: MUX_BETTER para elegir que use muxs en lugar de tri-states.
--               Por defecto es falso con lo que ahorra unos 12 slice.
--               * Agregado: FULL_SYNC para lograr el comportamiento original con 1 WS.
--               * Agregado: FIXED_PRER con lo que se puede fijar el valor del prescaler lo
--               que ahorra unos 11 slice.
--               * Modificado: Los case de lectura/escritura de los registros por if/elsif
--               que permite controlar mejor el uso de los generic.
--               * Modificado: El testbench para que soporte FIXED_PRER.
--
--               Revision 1.5  2005/03/09 20:32:24  salvador
--               * Arreglado: Colisi entre los nombres de las constantes y las seles.
--               XST tiene un bug que lo hace volverse loco con esto.
--
--               Revision 1.4  2005/03/09 19:24:32  salvador
--               * Agregado: Script para generar un .h y un .inc a partir del package
--               exportando los neros de los registros.
--               * Modificado: Para que los registros PRER_LO/HI no sean 0 y 1 sino 3 y 4.
--               * Corregido: El core para no usar "magics" sino los valores definidos en
--               el package para los neros de registros.
--               * Verificado con el testbench del core y del PIC.
--
--               Revision 1.3  2005/03/09 17:41:16  salvador
--               * Agregado: Hojas de datos del 24LC02B.
--               * Modificado: Reemplazo de Report por Assert porque las herramientas de
--               Xilinx no lo soportan.
--               * Modificado: Comentado los printf en core porque no tengo el equivalente
--               para Xilinx.
--               * Corregido: El TB de la memoria no contestaba ACK luego de la escritura.
--               Ahora si y adem el TB verifica que no falten ACKs.
--
--               Revision 1.2  2005/03/08 20:42:40  salvador
--               * Corregido: El core I2C insertaba un estado de espera en el Wishbone,
--               eliminado. Al mismo tiempo la sel TIP estaba siendo generada con un F/F
--               en lugar de ser combinacional (no es necesario ya que CR se borra con RST).
--               Ambos cambios hacen que el core use so 1 clock para Wishbone y reducen
--               en 2 F/F el uso (estimado, no verificado).
--               * Agregado: Generic DEBUG al core y que cuando esthabilitado informe las
--               lecturas y escrituras Wishbone.
--
--               Revision 1.1  2005/03/08 15:57:36  salvador
--               * Movido al repositorio CVS.
--               * Agregado: TestBench en VHDL.
--
--               Revision 1.7  2004/03/14 10:17:03  rherveille
--               Fixed simulation issue when writing to CR register
--
--               Revision 1.6  2003/08/09 07:01:13  rherveille
--               Fixed a bug in the Arbitration Lost generation caused by delay on the (external) sda line.
--               Fixed a potential bug in the byte controller's host-acknowledge generation.
--
--               Revision 1.5  2003/02/01 02:03:06  rherveille
--               Fixed a few 'arbitration lost' bugs. VHDL version only.
--
--               Revision 1.4  2002/12/26 16:05:47  rherveille
--               Core is now a Multimaster I2C controller.
--
--               Revision 1.3  2002/11/30 22:24:37  rherveille
--               Cleaned up code
--
--               Revision 1.2  2001/11/10 10:52:44  rherveille
--               Changed PRER reset value from 0x0000 to 0xffff, conform specs.
--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
library work;
use work.I2C_Master.all;
--synopsys translate off
library c;
use c.stdio_h.all;
--synopsys translate on

entity I2C_MasterTop is
   generic(
      ARST_LVL        : std_logic := '0';                -- asynchronous reset level
      DEBUG           : boolean := false;                -- enable debug registers
      MUX_BETTER      : boolean := false;                -- true if using MUX is better than using tri-states
      FULL_SYNC       : boolean := false;                -- true if you need full synchronous behavior, introduces 1 WS
      FIXED_PRER      : integer := -1;                   -- assigning a value removes the PRER and uses it as pre-scaler
      USE_IEN         : boolean := true                  -- false if interrupts are always enabled (masked in another component)
      );
   port (
      -- wishbone signals
      wb_clk_i        : in  std_logic;                    -- master clock input
      wb_rst_i        : in  std_logic := '0';             -- synchronous active high reset
      arst_i          : in  std_logic := not ARST_LVL;    -- asynchronous reset
      wb_adr_i        : in  unsigned(2 downto 0);         -- lower address bits
      wb_dat_i        : in  std_logic_vector(7 downto 0); -- Databus input
      wb_dat_o        : out std_logic_vector(7 downto 0); -- Databus output
      wb_we_i         : in  std_logic;                    -- Write enable input
      wb_stb_i        : in  std_logic;                    -- Strobe signals / core select signal
      wb_cyc_i        : in  std_logic:='1';               -- Valid bus cycle input. Optional. Needed?
      wb_ack_o        : out std_logic;                    -- Bus cycle acknowledge output
      wb_inta_o       : out std_logic;                    -- interrupt request output signal

      -- i2c lines
      scl_pad_i       : in  std_logic;                -- i2c clock line input
      scl_pad_o       : out std_logic;                -- i2c clock line output
      scl_padoen_o    : out std_logic;                -- i2c clock line output enable, active low
      sda_pad_i       : in  std_logic;                -- i2c data line input
      sda_pad_o       : out std_logic;                -- i2c data line output
      sda_padoen_o    : out std_logic                 -- i2c data line output enable, active low
      );
end entity I2C_MasterTop;

architecture Structural of I2C_MasterTop is
   component I2C_MasterByteCtrl is
      port (
         wb_clk_i        : in std_logic;
         wb_rst_i        : in std_logic; -- synchronous active high reset (WISHBONE compatible)
         nreset_i        : in std_logic;  -- asynchornous active low reset (FPGA compatible)
         ena_i           : in std_logic; -- core enable signal

         clk_cnt_i       : in unsigned(15 downto 0);     -- 4x SCL

         -- input signals
         start_i,
         stop_i,
         read_i,
         write_i,
         ack_in_i        : in std_logic;
         din_i           : in std_logic_vector(7 downto 0);

         -- output signals
         cmd_ack_o       : out std_logic;
         ack_out_o       : out std_logic;
         i2c_busy_o      : out std_logic;
         i2c_al_o        : out std_logic;
         dout_o          : out std_logic_vector(7 downto 0);

         -- i2c lines
         scl_i           : in std_logic;  -- i2c clock line input
         scl_o           : out std_logic; -- i2c clock line output
         scl_oen_o       : out std_logic; -- i2c clock line output enable, active low
         sda_i           : in std_logic;  -- i2c data line input
         sda_o           : out std_logic; -- i2c data line output
         sda_oen_o       : out std_logic  -- i2c data line output enable, active low
         );
   end component I2C_MasterByteCtrl;

   -- registers
   signal prer            : unsigned(15 downto 0);             -- clock prescale register
   signal ctr             : std_logic_vector(7 downto 6);      -- control register
   signal txr             : std_logic_vector(7 downto 0);      -- transmit register
   signal rxr             : std_logic_vector(7 downto 0);      -- receive register
   signal cr              : std_logic_vector(7 downto 2);      -- command register
   signal sr              : std_logic_vector(7 downto 0);      -- status register

   -- internal reset signal
   signal irst_i          : std_logic;

   -- wishbone write access
   signal wb_wacc         : std_logic;

   -- internal acknowledge signal
   signal iack_o          : std_logic;

   -- done signal: command completed, clear command register
   signal done            : std_logic;

   -- command register signals
   signal sta             : std_logic;
   signal sto             : std_logic;
   signal rd              : std_logic;
   signal wr              : std_logic;
   signal ack             : std_logic;
   signal iack            : std_logic;

   signal core_en         : std_logic;                -- core enable signal
   signal ien             : std_logic;                -- interrupt enable signal

   -- status register signals
   signal irxack, rxack   : std_logic;                -- received aknowledge from slave
   signal tip             : std_logic;                -- transfer in progress
   signal irq_flag        : std_logic;                -- interrupt pending flag
   signal i2c_busy_o      : std_logic;                -- i2c bus busy (start signal detected)
   signal i2c_al_o, al_o  : std_logic;                -- arbitration lost

begin
   -- generate internal reset signal
   irst_i <= arst_i xor ARST_LVL;

   -- generate acknowledge output signal
   gen_ack_o_ws:
   if FULL_SYNC generate
      gen_ack_o:
      process(wb_clk_i)
      begin
         if rising_edge(wb_clk_i) then
            iack_o <= wb_cyc_i and wb_stb_i and not iack_o;         -- because timing is always honored
         end if;
      end process gen_ack_o;
   end generate gen_ack_o_ws;

   gen_ack_o:
   if not(FULL_SYNC) generate
      -- SET: The above code generates 1 WS in the Wishbone bus.
      -- The following should be enough.
      iack_o <= wb_cyc_i and wb_stb_i;
   end generate gen_ack_o;

   wb_ack_o <= iack_o;
   -- end of acknowledge output signal

   -- generate wishbone write access signal
   wb_wacc <= wb_cyc_i and wb_stb_i and wb_we_i;

   -- pre-scaler register
   -- when FIXED_PRER is assigned we use it as divisor
   fixed_prer_assign:
   if not(FIXED_PRER=-1) generate
      prer <= to_unsigned(FIXED_PRER,16);
   end generate fixed_prer_assign;

   -- generate pre-scaler register
   prer_assign:
   if FIXED_PRER=-1 generate
      gen_prer:
      process(irst_i, wb_clk_i)
      begin
         if (irst_i = '0') then
            prer <= (others => '1');
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i = '1') then
               prer <= (others => '1');
            elsif (wb_wacc = '1') then
               if wb_adr_i=I2C_UPRER_LO then
                  prer(7 downto 0) <= unsigned(wb_dat_i);
               elsif wb_adr_i=I2C_UPRER_HI then
                  prer(15 downto 8) <= unsigned(wb_dat_i);
               end if;
            end if;
         end if;
      end process gen_prer;
   end generate prer_assign;
   -- end of pre-scaler register

   -------------------------------------------------------------------------
   -- Register decoding. Two versions one for tri-states (less cells for
   -- FPGAs) and another using muxs.
   -------------------------------------------------------------------------
   reg_decoder_bus:
   if not(MUX_BETTER) generate
      wb_dat_o <= std_logic_vector(prer( 7 downto 0))
        when wb_adr_i=I2C_UPRER_LO and FIXED_PRER=-1
        else (others => 'Z');
      wb_dat_o <= std_logic_vector(prer(15 downto 8))
        when wb_adr_i=I2C_UPRER_HI and FIXED_PRER=-1
        else (others => 'Z');
      wb_dat_o(ctr'range) <= ctr
        when wb_adr_i=I2C_UCTR
        else (others => 'Z');
      wb_dat_o <= rxr
        when wb_adr_i=I2C_URXR
        else (others => 'Z');
      wb_dat_o <= sr(7 downto 5) & "000" & sr(1 downto 0)
        when wb_adr_i=I2C_USR
        else (others => 'Z');
      wb_dat_o <= txr
        when wb_adr_i=I2C_UTXR_R and DEBUG
        else (others => 'Z');
      wb_dat_o(cr'range) <= cr
        when wb_adr_i=I2C_UCR_R and DEBUG
        else (others => 'Z');
   end generate reg_decoder_bus;

   reg_decoder_mux:
   if MUX_BETTER generate
      -- assign wb_dat_o
      assign_dato:
      process(wb_clk_i)
         variable dat_o : std_logic_vector(7 downto 0);
      begin
         if rising_edge(wb_clk_i) then
            dat_o := (others => '0');
            if wb_adr_i=I2C_UPRER_LO and FIXED_PRER=-1 then
               dat_o := std_logic_vector(prer(7 downto 0));
            elsif wb_adr_i=I2C_UPRER_HI and FIXED_PRER=-1 then
               dat_o := std_logic_vector(prer(15 downto 8));
            elsif wb_adr_i=I2C_UCTR then
               dat_o(ctr'range) := ctr;
            elsif wb_adr_i=I2C_URXR then
               dat_o := rxr; -- write is transmit register TxR
            elsif wb_adr_i=I2C_USR then
               dat_o := sr(7 downto 5) & "000" & sr(1 downto 0);  -- write is command register CR
               -- Debugging registers:
               -- These registers are not documented.
               -- Functionality could change in future releases
            elsif wb_adr_i=I2C_UTXR_R and DEBUG then
               dat_o := txr;
            elsif wb_adr_i=I2C_UCR_R and DEBUG then
               dat_o(cr'range) := cr;
            elsif wb_adr_i=I2C_UXXX_R and DEBUG then
               dat_o := (others => '0');
            else
               dat_o := (others => 'X');   -- for simulation only
            end if;
         end if;
         wb_dat_o <= dat_o;
         --synopsys translate off
         if DEBUG and wb_cyc_i='1' and wb_stb_i='1' and wb_we_i='0' then
            printf("Reading register %d ",to_integer(unsigned(wb_adr_i)));
            printf("(=0x%X)\n",to_integer(unsigned(dat_o)));
         end if;
         --synopsys translate on
      end process assign_dato;
   end generate reg_decoder_mux;
   -- end of register decoding


   -- generate registers (CR, SR see below)
   gen_regs:
   process(irst_i, wb_clk_i)
   begin
      if (irst_i = '0') then
         ctr  <= (others => '0');
         txr  <= (others => '0');
      elsif rising_edge(wb_clk_i) then
         if (wb_rst_i = '1') then
            ctr  <= (others => '0');
            txr  <= (others => '0');
         elsif (wb_wacc = '1') then
            if wb_adr_i=I2C_UPRER_LO and FIXED_PRER=-1 then
               null; --write to CR, avoid executing the others clause
            elsif wb_adr_i=I2C_UPRER_HI and FIXED_PRER=-1 then
               null; --write to PRER, avoid executing the others clause
            elsif wb_adr_i=I2C_UCTR then
               ctr <= wb_dat_i(ctr'range);
            elsif wb_adr_i=I2C_UTXR then
               txr <= wb_dat_i;
            elsif wb_adr_i=I2C_UCR then
               null; --write to CR, avoid executing the others clause
            else -- illegal cases, for simulation only
               --synopsys translate off
               assert false
               report "Illegal write address, setting all registers to unknown."
               severity failure;
               --synopsys translate on
               ctr  <= (others => 'X');
               txr  <= (others => 'X');
            end if;
            --synopsys translate off
            if DEBUG then
               printf("Writing register %d ",to_integer(unsigned(wb_adr_i)));
               printf(" with 0x%X\n",to_integer(unsigned(wb_dat_i)));
            end if;
            --synopsys translate on
         end if;
      end if;
   end process gen_regs;


   -- generate command register
   gen_cr:
   process(irst_i, wb_clk_i)
   begin
      if (irst_i = '0') then
         cr <= (others => '0');
      elsif rising_edge(wb_clk_i) then
         if (wb_rst_i = '1') then
            cr <= (others => '0');
         elsif (wb_wacc = '1') then
            if ( (core_en = '1') and (wb_adr_i = I2C_UCR) ) then
               -- only take new commands when i2c core enabled
               -- pending commands are finished
               cr <= wb_dat_i(cr'range);
            end if;
         else
            if (done = '1' or i2c_al_o= '1') then
               cr(7 downto 4) <= (others => '0'); -- clear command bits when command done or arbitration lost
            end if;
            cr(2) <= '0';                        -- clear IRQ_ACK bit
         end if;
      end if;
   end process gen_cr;

   -- decode command register
   sta  <= cr(7);
   sto  <= cr(6);
   rd   <= cr(5);
   wr   <= cr(4);
   ack  <= cr(3);
   iack <= cr(2);


   -- TIP bit generation
   gen_tip:
   if not(FULL_SYNC) generate
      -- SET: tip is just (rd or wr), the reset signals affects cr, we don't
      -- need to generate another F/F for it.
      tip  <= (rd or wr);
   end generate gen_tip;

   gen_tip_sync:
   if FULL_SYNC generate
      gen_tip_proc:
      process (wb_clk_i, irst_i)
      begin
         if (irst_i = '0') then
            tip <= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i = '1') then
               tip <= '0';
            else
               tip <= (rd or wr);
            end if;
         end if;
      end process gen_tip_proc;
   end generate gen_tip_sync;
   -- end of TIP bit generation


   -- decode control register
   core_en <= ctr(7);
   ien     <= ctr(6) when USE_IEN else '1';

   -- hookup byte controller block
   byte_ctrl: I2C_MasterByteCtrl
      port map(
         wb_clk_i => wb_clk_i,
         wb_rst_i => wb_rst_i,
         nreset_i => irst_i,
         ena_i    => core_en,
         clk_cnt_i=> prer,
         start_i  => sta,
         stop_i   => sto,
         read_i   => rd,
         write_i  => wr,
         ack_in_i => ack,
         i2c_busy_o=> i2c_busy_o,
         i2c_al_o => i2c_al_o,
         din_i    => txr,
         cmd_ack_o=> done,
         ack_out_o=> irxack,
         dout_o   => rxr,
         scl_i    => scl_pad_i,
         scl_o    => scl_pad_o,
         scl_oen_o=> scl_padoen_o,
         sda_i    => sda_pad_i,
         sda_o    => sda_pad_o,
         sda_oen_o=> sda_padoen_o
         );


   -- status register block + interrupt request signal
   st_irq_block:
   block
   begin
      -- generate status register bits
      gen_sr_bits:
      process (wb_clk_i, irst_i)
      begin
         if (irst_i = '0') then
            al_o     <= '0';
            rxack    <= '0';
            irq_flag <= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i = '1') then
               al_o     <= '0';
               rxack    <= '0';
               irq_flag <= '0';
            else
               al_o     <= i2c_al_o or (al_o and not sta);
               rxack    <= irxack;

               -- interrupt request flag is always generated
               irq_flag <= (done or i2c_al_o or irq_flag) and not iack;
            end if;
         end if;
      end process gen_sr_bits;

      -- generate interrupt request signals
      gen_irq:
      process (wb_clk_i, irst_i)
      begin
         if (irst_i = '0') then
            wb_inta_o <= '0';
         elsif rising_edge(wb_clk_i) then
            if (wb_rst_i = '1') then
               wb_inta_o <= '0';
            else
               -- interrupt signal is only generated when IEN (interrupt enable bit) is set
               wb_inta_o <= irq_flag and ien;
            end if;
         end if;
      end process gen_irq;

      -- assign status register bits
      sr(7)          <= rxack;
      sr(6)          <= i2c_busy_o;
      sr(5)          <= al_o;
      --sr(4 downto 2) <= (others => '0'); -- reserved
      sr(1)          <= tip;
      sr(0)          <= irq_flag;
   end block st_irq_block;

end architecture Structural;
