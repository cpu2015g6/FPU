library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

entity finv is
  port(clk:       in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end finv;

architecture VHDL of finv is

  component encode -- opのstatusをencode
  port(op:  in std_logic_vector(31 downto 0);
       code:  out std_logic_vector(2 downto 0)
       );
end component;


  component fmul_inv
  port(clk: in std_logic;
       romdata:  in std_logic_vector(22 downto 0); --23bit
       del: in std_logic_vector(11 downto 0); --12bit
       flag: in std_logic;
       ans:  out std_logic_vector(22 downto 0) -- dataと引き算するため23bit
       );
end component;

signal x1,x2,x3,x4,x5,x6: std_logic_vector(31 downto 0) := (others=>'0');
signal man: std_logic_vector(22 downto 0) := (others=>'0');
signal tb1,tb2,tb3,tb4,tb5,tb6: std_logic_vector(22 downto 0);
signal data,data2,mulans: std_logic_vector(22 downto 0);
signal flag,sgn: std_logic;
signal exp: std_logic_vector(7 downto 0);
signal code1: std_logic_vector(2 downto 0);
signal state: std_logic_vector(4 downto 0) := "00000"; --wait state

begin

  opch1:encode
    port map(op,code1);

  mul_inv:fmul_inv
    port map(clk,data2,del,flag,mulans)


man<= op(22 downto 0);
exp<="11111101" - op(30 downto 23);
  
    loop24:process(clk)
      begin
        if rising_edge(clk) then
          if state="00000" then
            state<=state+1;
            if op(22) = '0' then
              x1<=op(31) & exp & "10000000000000000000000";
              tb<=man + ("1" & op(22 downto 1));
            else
              x1<=op(31) & exp & "00000000000000000000000";
              tb<=man;
            end if;
          elsif state="00001" then
            state<=state+1;
            if tb(22)='0' then
              


 
exp<="11111101" - op(30 downto 23);
ans<=sgn & exp & (data - mulans) when code1(2 downto 1) = "00" else
     "11111111110000000000000000000000" when code1 = "111" else
     sgn & "1111111100000000000000000000000" when code < 5 else
     sgn & "0000000000000000000000000000000";
