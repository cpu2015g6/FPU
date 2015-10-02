library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;


entity non_norm is
  port(op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end non_norm;

architecture VHDL of non_norm is
begin


ans<=op1(31) & (op1(30 downto 0) + op2(30 downto 0)) when op1(31) = op2(31) else
     op1(31 downto 23) & (op1(22 downto 0) - op2(22 downto 0)) when op1(30 downto 0) > op2(30 downto 0) else
     op2(31 downto 23) & (op2(22 downto 0) - op1(22 downto 0));

end VHDL;
