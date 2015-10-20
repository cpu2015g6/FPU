library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity encode is
  port(op:  in std_logic_vector(31 downto 0);
       code:  out std_logic_vector(2 downto 0)
       );
end encode;

architecture VHDL of encode is
-- encode the status of operand
-- 000 +Inf   100 -Inf   001 NaN   101 Zero
-- 010 Positive Denormalized Number   110 Negative Denormalized Number 
-- 000 Positive Normalized Number   001 Negative Normalized Number

begin

  code <= "100" when op(31 downto 23) = "01111111100000000000000000000000" else -- +inf
         "101" when op(31 downto 23) = "11111111100000000000000000000000" else -- -inf
         "111" when op(30 downto 23) = "11111111" else -- NaN
         "110" when op(30 downto 0) = "0000000000000000000000000000000" else -- zero
         "010" when op(31 downto 23) = "000000000" else -- pos denorm num
	 "011" when op(31 downto 23) = "100000000" else -- neg denorm num
         "000" when op(31) = '0' else -- positive
         "001";
   end VHDL;
