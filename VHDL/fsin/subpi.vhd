library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返してほしい
entity subpi is
  port(clk:       in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"0000"
       );
end subpi;

architecture VHDL of subpi is
  signal ad1,ad2,ad3,ad4,ad5,ad6,ad7,ad8: std_logic_vector(7 downto 0) := (others=>'0');

