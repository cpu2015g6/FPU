library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 3clkで答を返す
entity finv is
  port(clk:  in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(31 downto 0) := x"00000000"
       );
end finv;

architecture VHDL of finv is

  component invblockram1
    generic (
    dwidth : integer := 23;
    awidth : integer := 10);
    port (
    clk  : in  std_logic;
    we   : in  std_logic;
    di   : in  std_logic_vector(dwidth - 1 downto 0);
    do   : out std_logic_vector(dwidth - 1 downto 0);
    addr : in  std_logic_vector(awidth - 1 downto 0));
end component;

  component invblockram2
    generic (
    dwidth : integer := 13;
    awidth : integer := 10);
    port (
    clk  : in  std_logic;
    we   : in  std_logic;
    di   : in  std_logic_vector(dwidth - 1 downto 0);
    do   : out std_logic_vector(dwidth - 1 downto 0);
    addr : in  std_logic_vector(awidth - 1 downto 0));
end component;

signal din,do2: std_logic_vector(12 downto 0);
signal do1: std_logic_vector(22 downto 0);
signal addr: std_logic_vector(9 downto 0);
signal data1,data2,init,ainit: std_logic_vector(22 downto 0);
signal flag,we,sgn,asgn,aasgn: std_logic;
signal del1,del2,adel1,adel2: std_logic_vector(6 downto 0);
signal exp,aexp,aaexp: std_logic_vector(7 downto 0);

begin

  rom1:invblockram1
    port map(clk,we,din,do1,addr);

  rom1:invblockram2
    port map(clk,we,din,do2,addr);

sgn<=op(31);
del1<=op(13 downto 7);
del2<=op(6 downto 0);
we<='0';
addr<=op(22 downto 13);
flag<= '1' when addr > 425 else '0';
exp<=253 - op(30 downto 23);

  pipe1:process(clk)
  begin
    if rising_edge(clk) then
      asgn<=sgn;
      aexp<=exp;
      aflag<=flag;
      adel1<=del1;
      adel2<=del2;
      init<=do1;
      if flag = '0' then
        data1<="1" & do2(12 downto 7);
        data2<=do2(6 downto 0);
      else
        data1<="01" & do2(12 downto 8);
        data2<=do2(7 downto 1);
    end if;
  end process;

mul1<=adel1*data1;
mul2<=adel1*data2(13 downto 7);
mul3<=adel2*data1(13 downto 7);

  pipe2:process(clk)
  begin
    if rising_edge(clk) then
      aasgn<=asgn;
      ainit<=init;
      aaexp<=aexp;
      amul1<="000000000" & mul1;
      amul2<="0000000000000000" & mul2(13 downto 7);
      amul3<="0000000000000000" & mul3(13 downto 7);
    end if;
  end process;
	 
ans<= aasgn & aaexp & (ainit - (amul1 + amul2 + amul3));

end VHDL;
