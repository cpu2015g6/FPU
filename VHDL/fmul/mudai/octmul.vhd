library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返してほしい
entity octmul is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(7 downto 0);
       ans:       out std_logic_vector(15 downto 0) := x"0000"
       );
end octmul;

architecture VHDL of octmul is
  signal ad1,ad2,ad3,ad4,ad5,ad6,ad7,ad8: std_logic_vector(7 downto 0) := (others=>'0');

begin
  
--ad1<=op1 when op2(7) = '1' else x"00";
--ad2<=op1 when op2(6) = '1' else x"00";
--ad3<=op1 when op2(5) = '1' else x"00";
--ad4<=op1 when op2(4) = '1' else x"00";
--ad5<=op1 when op2(3) = '1' else x"00";
--ad6<=op1 when op2(2) = '1' else x"00";
--ad7<=op1 when op2(1) = '1' else x"00";
--ad8<=op1 when op2(0) = '1' else x"00";



--ans<=("0" & (ad1 & "0000000")) +
     --("00" & (ad2 & "000000")) +
     --("000" & (ad3 & "00000")) +
     --("0000" & (ad4 & "0000")) +
     --("00000" & (ad5 & "000")) +
     --("000000" & (ad6 & "00")) +
     --("0000000" & (ad7 & "0")) +
     --("00000000" & ad8);

ans<=op1*op2;

end VHDL;
