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
  signal a1,aa1,aaa1,a2,m2,m3,m4,m5,m7,r3,r5,r7,ad13,aad13,ad57 : std_logic_vector(31 downto 0);

component fadd--2clk
  port(clk: in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul--1clk
  port(clk: in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul_3--1clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul_5--1clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component fmul_7--1clk
  port(clk: in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;
  
begin

  fadder1:fadd port map
    (clk, aaa1, r3, ad13);

  fadder2:fadd port map
    (clk, r5, r7, ad57);

  fadder3:fadd port map
    (clk, aad13, ad57, ans);

  fmuler1:fmul port map
    (clk, op, op, m2);

  fmuler2:fmul port map
    (clk, a1, m2, m3);

  fmuler3:fmul port map
    (clk, m2, m2, m4);

  fmuler4:fmul port map
    (clk, a2, m3, m5);

  fmuler5:fmul port map
    (clk, m3, m4, m7);

  fmuler6:fmul_3 port map
    (clk, m3, r3);

  fmuler6:fmul_5 port map
    (clk, m5, r5);

  fmuler6:fmul_7 port map
    (clk, m7, r7);


  dan1:process(clk)
    begin
      if rising_edge(clk) then
        a1<=op;
      end if;
    end process;

  dan2:process(clk)
    begin
      if rising_edge(clk) then
        aa1<=a1;
        a2<=m2;
      end if;
    end process;

  dan3:process(clk)
    begin
      if rising_edge(clk) then
        aaa1<=aa1;
      end if;
    end process;

  end VHDL;
