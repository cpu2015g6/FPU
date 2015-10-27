library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返す+1
entity fadd_2 is
  port(clk:       in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fadd_2;

architecture VHDL of fadd_2 is

component revfadd
  port(clk:       in std_logic;
       op1,op2: in std_logic_vector(31 downto 0);
       ans: out std_logic_vector(31 downto 0));
  end component;

begin

  chan2:revfadd
    port map(clk,op,op2,revans);

op2<="00111111100000000000000000000000";
ans<=revans;

end VHDL;
 
