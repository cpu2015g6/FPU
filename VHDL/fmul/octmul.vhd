library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返してほしい
entity octmul is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(7 downto 0);
       ans:       out std_logic_vector(15 downto 0) := x"00000000"
       );
end octmul;

architecture VHDL of octmul is
  signal ad1,ad2,ad3,ad4,ad5,ad6,ad7,ad8: std_logic_vector(7 downto 0) := (others=>'0');

begin
  
ad1<=op1 when op(7) = '1' else x"00";
ad2<=op1 when op(6) = '1' else x"00";
ad3<=op1 when op(5) = '1' else x"00";
ad4<=op1 when op(4) = '1' else x"00";
ad5<=op1 when op(3) = '1' else x"00";
ad6<=op1 when op(2) = '1' else x"00";
ad7<=op1 when op(1) = '1' else x"00";
ad8<=op1 when op(0) = '1' else x"00";

ans<=("0" & (op1 & "0000000")) +
     ("00" & (op2 & "000000")) +
     ("000" & (op3 & "00000")) +
     ("0000" & (op4 & "0000")) +
     ("00000" & (op5 & "000")) +
     ("000000" & (op6 & "00")) +
     ("0000000" & (op5 & "0")) +
     ("00000000" & op6);

end VHDL;
