library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返す
entity fsqrt is
  port(clk:  in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(31 downto 0) := x"00000000"
       );
end fsqrt;

architecture VHDL of fsqrt is

  component encode -- opのstatusをencode
  port(op:  in std_logic_vector(31 downto 0);
       code:  out std_logic_vector(2 downto 0)
       );
end component;

  component sqrtromfetch 
  port(addr:  in std_logic_vector(10 downto 0); --11bit
       data,data2:  out std_logic_vector(22 downto 0) -- 23bit
       );
end component;

  component fmul_sqrt
  port(clk: in std_logic;
       romdata:  in std_logic_vector(22 downto 0); --23bit
       del: in std_logic_vector(12 downto 0); --12bit
       flag: in std_logic;
       ans:  out std_logic_vector(22 downto 0) -- dataと引き算するため23bit
       );
end component;

signal addr: std_logic_vector(10 downto 0);
signal data,data2,mulans: std_logic_vector(22 downto 0);
signal del: std_logic_vector(12 downto 0);
signal flag: std_logic;
signal exp: std_logic_vector(7 downto 0);
signal code1: std_logic_vector(2 downto 0);

begin

  opch1:encode
    port map(op,code1);

  fetch:sqrtromfetch
    port map(addr,data,data2);

  mul_inv:fmul_sqrt
    port map(clk,data2,del,flag,mulans);

del<=op(12 downto 0);
addr<=op(23 downto 13);
flag<=addr(10);
exp<=("0" & op(30 downto 24)) + 63 + op(23);
ans<="0" & exp & (data + mulans) when code1(2 downto 1) = "00" else
     "11111111110000000000000000000000" when code1 = "111" else
     "00000000000000000000000000000000" when code1="110" else
     "11111111110000000000000000000000";

end VHDL;
