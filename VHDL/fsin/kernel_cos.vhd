library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 8clkで返す
 entity kernel_sin
  port(clk: in std_logic;
       op: in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(31 downto 0)
       );
end kernel_sin;

architecture VHDL of kernel_sin is
  signal a2,m2,m4,m6,r2,r4,r6,ar4,ad02,aad02,aaad02,ad46 : std_logic_vector(31 downto 0);

component fadd--2clk
  port(clk: in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fadd_2--2clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul--1clk
  port(clk: in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul_2--1clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul_4--1clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul_6--1clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;
  
begin

  fadder1:fadd_2 port map
    (clk, r2, ad02);

  fadder2:fadd port map
    (clk, r6, ar4, ad46);

  fadder3:fadd port map
    (clk, aaad02, ad46, ans);

  fmuler1:fmul port map
    (clk, op, op, m2);

  fmuler2:fmul port map
    (clk, m2, m2, m4);

  fmuler3:fmul port map
    (clk, a2, m4, m6);

  fmuler4:fmul_2 port map
    (clk, m2, r2);

  fmuler5:fmul_4 port map
    (clk, m4, r4);

  fmuler6:fmul_6 port map
    (clk, m6, r6);

  dan1:process(clk)
    begin
      if rising_edge(clk) then
        a2<=m2;
      end if;
    end process;

  dan2:process(clk)
    begin
      if rising_edge(clk) then
        ar4<=a4;
      end if;
    end process;

  end VHDL;
