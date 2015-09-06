------------------------------------------------------------------------------
----                                                                      ----
----  I2C Memory Simulator (slave)                                        ----
----                                                                      ----
----  Internal file, can't be downloaded.                                 ----
----                                                                      ----
----  Description:                                                        ----
----  I2C memory simulator package.                                       ----
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
---- Copyright (c) 2005 Instituto Nacional de Tecnología Industrial       ----
----                                                                      ----
---- Covered by the GPL license.                                          ----
----                                                                      ----
------------------------------------------------------------------------------
----                                                                      ----
---- Design unit:      I2C_TB (Package)                                   ----
---- File name:        i2c_memory_pkg.vhdl                                ----
---- Note:             None                                               ----
---- Limitations:      None known                                         ----
---- Errors:           None known                                         ----
---- Library:          i2c_mwb                                            ----
---- Dependencies:     IEEE.std_logic_1164                                ----
----                   IEEE.numeric_std                                   ----
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
-- $Log: i2c_memory_pkg.vhdl,v $
-- Revision 1.5  2006/04/17 19:44:43  salvador
-- * Modified: License to GPL.
--
-- Revision 1.4  2005/05/20 14:39:05  salvador
-- * Modificado: Mejorado el indentado usando bakalint 0.3.7.
--
-- Revision 1.3  2005/05/18 14:50:20  salvador
-- * Modificado: Los encabezados de los archivos para que cumplan con nuestras
-- recomendaciones.
--
--
--

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package I2C_TB is
   component I2C_Memory is
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
   end component I2C_Memory;
end package I2C_TB;

