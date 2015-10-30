library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返す
-- rt <- if ra > rb then 2 elseif ra = rb then 1 else 0
entity fcmp is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(1 downto 0)
       );
end fcmp;

architecture VHDL of fcmp is
signal sgn : std_logic;

begin

sgn<=op1(31) xor op2(31);
ans<= "10" when sgn = '1' and op1(31) = '0' else
      "00" when sgn = '1' and op1(31) = '1' else
      "10" when op1 > op2 and op1(31) = '0' else
      "10" when op2 > op1 and op1(31) = '1' else
      "00" when op1 /= op2 else
      "01";

end VHDL;
