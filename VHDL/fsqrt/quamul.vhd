library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返してほしい
entity quamul is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(3 downto 0);
       ans:       out std_logic_vector(7 downto 0) := x"00"
       );
end quamul;

architecture VHDL of quamul is
  signal ad1,ad2,ad3,ad4: std_logic_vector(3 downto 0) := (others=>'0');

begin
  
ad1<=op1 when op2(3) = '1' else x"0";
ad2<=op1 when op2(2) = '1' else x"0";
ad3<=op1 when op2(1) = '1' else x"0";
ad4<=op1 when op2(0) = '1' else x"0";

ans<=("0" & (ad1 & "000")) +
     ("00" & (ad2 & "00")) +
     ("000" & (ad3 & "0")) +
     ("0000" & ad4);
end VHDL;
