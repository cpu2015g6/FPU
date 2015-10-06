library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

entity round1 is
  port(op : in std_logic_vector(31 downto 0);
       point : in std_logic_vector(24 downto 0);
       ans:  out std_logic_vector(31 downto 0) := (others=>'0')
       );
end round1;

architecture VHDL of round1 is
begin

  ans<= op when point(24) = '0' else
        op+1 when point(24) = '1' and point(23 downto 0) > 0 else
        op+1 when op(0) = '1' else
        op;

end VHDL;
