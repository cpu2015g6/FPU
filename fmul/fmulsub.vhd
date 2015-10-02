library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返す
entity fmulsub is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmulsub;

architecture VHDL of fmulsub is

  

begin


  end VHDL;
