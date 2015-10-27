library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity infcal is
  port(op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0)
       );
end infcal;

architecture VHDL of infcal is

 constant nan: std_logic_vector(31 downto 0) := "11111111110000000000000000000000";

begin

  ans <= nan when op1(22 downto 0) /= "00000000000000000000000" and op1(30 downto 23) = "11111111" else --op1 is nan
         nan when op2(22 downto 0) /= "00000000000000000000000" and op2(30 downto 23) = "11111111" else --op2 is nan
         nan when (op1(31) /= op2(31) and op1(30 downto 23) = "11111111") and op2(30 downto 23) = "11111111" else -- inf + -inf
         op1 when op1(30 downto 23) = "11111111" else
         op2;
   end VHDL;
