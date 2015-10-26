library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

--10clk
entity fsin is
  port(clk:       in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fsin;

architecture VHDL of fsin is

  component kernel_cos
  port(clk: in std_logic;
       op: in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(31 downto 0)
       );
end component;

  component kernel_sin
  port(clk: in std_logic;
       op: in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(31 downto 0)
       );
end component;

signal flag1,flag2,flag3,flag4,flag5,flag6,flag7,flag8,flag9,flag10: std_logic;
signal sgn1,sgn2,sgn3,sgn4,sgn5,sgn6,sgn7,sgn8,sgn9,sgn10: std_logic;
signal abso:std_logic_vector(31 downto 0);

begin

abso<="0" & op(30 downto 0);

  subpi:fadd_subpi
    port map(clk,abso,ans1);

  subpi2:fadd_subpi2
    port map(clk,abso,ans2);

  kc:kernel_cos
    port map(clk,cosk,anskc);

  ks:kernel_sin
    port map(clk,sink,ansks);

abso<="0" & op(30 downto 0);
sgn1<=op(31);
flag1<='1' when abso < x"3f490fda" else '0';
       
  
  process(clk)
    begin
      if rising_edge(clk) then
        cosk<=addans;
        sink<=addans;
        sgn2<=sgn1;
        sgn3<=sgn2;
        sgn4<=sgn3;
        sgn5<=sgn4;
        sgn6<=sgn5;
        sgn7<=sgn6;
        sgn8<=sgn7;
        sgn9<=sgn8;
        sgn10<=sgn9;
        flag2<=flag1;
        flag3<=flag2;
        flag4<=flag3;
        flag5<=flag4;
        flag6<=flag5;
        flag7<=flag6;
        flag8<=flag7;
        flag9<=flag8;
        flag10<=flag9;
        if 
        if flag10 = '1' then
          ans<=sgn10 & ansks(30 downto 0);
        else
          ans<=sgn10 & anskc(30 downto 0);
        end if;
      end process;
	  
end VHDL;
