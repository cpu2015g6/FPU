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
signal data,adata,data2,mulans: std_logic_vector(22 downto 0);
signal del: std_logic_vector(12 downto 0);
signal flag,exf,exf2: std_logic;
signal exp,exp2: std_logic_vector(7 downto 0);

begin

  fetch:sqrtromfetch
    port map(addr,data,data2);

  mul_inv:fmul_sqrt
    port map(clk,data2,del,flag,mulans);
	 
del<=op(12 downto 0);
addr<=op(23 downto 13);
flag<=addr(10);
exf<='1' when addr = "10000000000" else '0';
exp<=("0" & op(30 downto 24)) + 63 + op(23);

  process(clk)
    begin
      if rising_edge(clk) then
        exp2<=exp;
        adata<=data;
		  exf2<=exf;
      end if;
    end process;
	 
ans<="0" & exp2 & (adata + mulans) when exf2 = '0' else
     "0" & exp2 & (adata + (mulans(21 downto 0) & "0"));

end VHDL;
