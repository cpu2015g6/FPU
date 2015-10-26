library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 4clkで答を返す
entity fdiv is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fdiv;

architecture VHDL of fdiv is

component finv--2clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul--2clk
  port(clk: in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

signal invans,a2,aa2: std_logic_vector(31 downto 0);

begin
  
   finver:finv port map
    (clk, op1, invans);

   fmuler:fmul port map
    (clk, aa2, invans, ans);

	process(clk)
	 begin
	  if rising_edge(clk) then
	   a2<=op2;
	   aa2<=a2;
	  end if;
	 end process;

end VHDL;
