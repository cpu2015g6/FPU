library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返す
entity fmulsub is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmulsub;

architecture VHDL of fmulsub is
  signal sgn:std_logic := '0';
  signal man:std_logic_vector(23 downto 0) := (others=>'0');
  signal result: std_logic_vector(46 downto 0) := (others=>'0');
  
begin
sgn<=op1(31) xor op2(31);
man<="1" & op2(22 downto 0);
result <= (op2(22 downto 0) & "000000000000000000000000") +
          (("0" & ())

  end VHDL;
