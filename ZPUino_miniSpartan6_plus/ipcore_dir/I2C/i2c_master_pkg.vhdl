------------------------------------------------------------------------------
----                                                                      ----
----  I2C Master Core Package                                             ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----                                                                      ----
----  Description:                                                        ----
----  I2C Master module. Implemented as a Wishbone Slave. (Wishbone to    ----
---- I2C bridge?). Package for the code by Richard Herveille.             ----
----                                                                      ----
----  To Do:                                                              ----
----  -                                                                   ----
----                                                                      ----
----  Author:                                                             ----
----    - Salvador E. Tropea, salvador en inti gov ar                     ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Copyright (c) 2005 Salvador E. Tropea <salvador en inti gov ar>      ----
---- Copyright (c) 2005 Instituto Nacional de Tecnolog Industrial       ----
----                                                                      ----
---- Covered by the GPL license.                                          ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Design unit:      I2C_Master (Package)                               ----
---- File name:        i2c_master_pkg.vhdl                                ----
---- Note:             None                                               ----
---- Limitations:      None known                                         ----
---- Errors:           None known                                         ----
---- Library:          i2c_mwb                                            ----
---- Dependencies:     IEEE.std_logic_1164                                ----
----                   IEEE.numeric_std                                   ----
---- Target FPGA:      Spartan II (XC2S100-5-PQ208)                       ----
---- Language:         VHDL                                               ----
---- Wishbone:         SLAVE (rev B.2)                                    ----
---- Synthesis tools:  Xilinx Release 6.2.03i - xst G.31a                 ----
---- Simulation tools: GHDL [Sokcho edition] (0.1x)                       ----
---- Text editor:      SETEdit 0.5.x                                      ----
----                                                                      ----
------------------------------------------------------------------------------
--
-- CVS Revision History
--
-- $Log: i2c_master_pkg.vhdl,v $
-- Revision 1.10  2006/04/17 19:44:43  salvador
-- * Modified: License to GPL.
--
-- Revision 1.9  2005/05/20 14:39:05  salvador
-- * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
-- Revision 1.8  2005/05/18 14:50:19  salvador
-- * Modificado: Los encabezados de los archivos para que cumplan con nuestras
-- recomendaciones.
--
--
--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package I2C_Master is
   component I2C_MasterTop is
      generic(
         ARST_LVL        : std_logic:='0'; -- asynchronous reset level
         DEBUG           : boolean:=false; -- enable debug registers
         MUX_BETTER      : boolean:=false; -- true if using MUX is better than using tri-states
         FULL_SYNC       : boolean:=false; -- true if you need full synchronous behavior, introduces 1 WS
         FIXED_PRER      : integer:=-1;    -- assigning a value removes the PRER and uses it as pre-scaler
         USE_IEN         : boolean:=true   -- false if interrupts are always enabled (masked in another component)
         );
      port(-- Wishbone signals
         wb_clk_i        : in  std_logic;                    -- master clock input
         wb_rst_i        : in  std_logic := '0';             -- synchronous active high reset
         arst_i          : in  std_logic := not ARST_LVL;    -- asynchronous reset
         wb_adr_i        : in  unsigned(2 downto 0);         -- lower address bits
         wb_dat_i        : in  std_logic_vector(7 downto 0); -- Databus input
         wb_dat_o        : out std_logic_vector(7 downto 0); -- Databus output
         wb_we_i         : in  std_logic;                    -- Write enable input
         wb_stb_i        : in  std_logic;                    -- Strobe signals / core select signal
         wb_cyc_i        : in  std_logic:='1';               -- Valid bus cycle input
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
   end component I2C_MasterTop;

   -- EXPORT CONSTANTS
   constant I2C_CTR       : std_logic_vector(7 downto 0):="00000000";
   constant I2C_RXR       : std_logic_vector(7 downto 0):="00000001";
   constant I2C_TXR       : std_logic_vector(7 downto 0):="00000001";
   constant I2C_CR        : std_logic_vector(7 downto 0):="00000010";
   constant I2C_SR        : std_logic_vector(7 downto 0):="00000010";
   constant I2C_PRER_LO   : std_logic_vector(7 downto 0):="00000011";
   constant I2C_PRER_HI   : std_logic_vector(7 downto 0):="00000100";
   -- END EXPORT CONSTANTS

   constant I2C_TXR_R     : std_logic_vector(7 downto 0):="00000101"; -- undocumented / reserved output
   constant I2C_CR_R      : std_logic_vector(7 downto 0):="00000110"; -- undocumented / reserved output
   constant I2C_XXX_R     : std_logic_vector(7 downto 0):="00000111"; -- undocumented / reserved output

   constant I2C_UCTR      : unsigned(2 downto 0):="000";
   constant I2C_URXR      : unsigned(2 downto 0):="001";
   constant I2C_UTXR      : unsigned(2 downto 0):="001";
   constant I2C_UCR       : unsigned(2 downto 0):="010";
   constant I2C_USR       : unsigned(2 downto 0):="010";
   constant I2C_UPRER_LO  : unsigned(2 downto 0):="011";
   constant I2C_UPRER_HI  : unsigned(2 downto 0):="100";

   constant I2C_UTXR_R    : unsigned(2 downto 0):="101"; -- undocumented / reserved output
   constant I2C_UCR_R     : unsigned(2 downto 0):="110"; -- undocumented / reserved output
   constant I2C_UXXX_R    : unsigned(2 downto 0):="111"; -- undocumented / reserved output
end package I2C_Master;

