library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 3clkで答を返す
entity fsqrt is
  port(clk:  in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(31 downto 0) := x"00000000"
       );
end fsqrt;

architecture VHDL of fsqrt is

  component blockram1
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

  component blockram2
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
signal mul1,mul2,mul3: std_logic_vector(13 downto 0);
signal amul1,amul2,amul3,din2: std_logic_vector(22 downto 0);
signal do1,init,ainit: std_logic_vector(22 downto 0);
signal addr: std_logic_vector(9 downto 0);
signal del1,del2,adel1,adel2,data1,data2: std_logic_vector(6 downto 0);
signal flag,aflag,aaflag,exf,aexf,we: std_logic;
signal exp,aexp,aaexp: std_logic_vector(7 downto 0);
signal pohe : std_logic_vector(22 downto 0);
begin

  rom1:blockram1
    port map(clk,we,din2,do1,addr);

  rom2:blockram2
    port map(clk,we,din,do2,addr);

del1<=op(13 downto 7);
del2<=op(6 downto 0);
we<='0';
addr<=op(23 downto 14);
flag<=addr(9);
exf<='1' when addr = "1000000000" else '0';
exp<=("0" & op(30 downto 24)) + 63 + op(23);

  pipe1:process(clk)
  begin
    if rising_edge(clk) then
      aexp<=exp;
      aflag<=flag;
      aexf<=exf;
      adel1<=del1;
      adel2<=del2;
    end if;
  end process;

init<=do1;
data1<="1" & do2(12 downto 7);
data2<=do2(6 downto 0);
mul1<=adel1*data1;
mul2<=adel1*data2;
mul3<=adel2*data1;

  pipe2:process(clk)
  begin
    if rising_edge(clk) then
      ainit<=init;
      aaexp<=aexp;
      aaflag<=aflag;
      if aexf = '0' then
        amul1<="000000000" & mul1;
        amul2<="0000000000000000" & mul2(13 downto 7);
        amul3<="0000000000000000" & mul3(13 downto 7);
      else
        amul1<="000000000" & adel1 & adel2;
        amul2<="00000000000000000000000";
        amul3<="00000000000000000000000";
      end if;
    end if;
  end process;
	 
pohe <= amul1 + amul2 + amul3;
ans<="0" & aaexp & (ainit + ("0" & pohe(22 downto 1))) when aaflag = '1' else
     "0" & aaexp & (ainit + pohe);

end VHDL;
