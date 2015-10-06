library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity encode is
  port(op:  in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(2 downto 0)
       );
end encode;

architecture VHDL of encode is


begin

  ans <= "000" when op(31 downto 23) = "011111111" and op(22 downto 0) = "00000000000000000000000" else -- +inf
         "100" when op(31 downto 23) = "111111111" and op(22 downto 0) = "00000000000000000000000" else -- -inf
         "001" when op(30 downto 23) = "11111111" else -- nan
         "101" when op(30 downto 0) = "0000000000000000000000000000000" else -- zero
         "010" when op(31 downto 23) = "000000000" else -- pos nonnorm
	 "110" when op(31 downto 23) = "100000000" else -- neg nonnorm
         "011" when op(31) = '0' else -- positive
         "111";
   end VHDL;
