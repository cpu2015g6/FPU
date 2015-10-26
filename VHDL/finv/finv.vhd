library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

--2clk
entity finv is
  port(clk:       in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end finv;

architecture VHDL of finv is

  component invromfetch 
  port(addr:  in std_logic_vector(10 downto 0); --11bit
       data,data2:  out std_logic_vector(22 downto 0) -- 23bit
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

signal addr: std_logic_vector(10 downto 0);
signal data,data2,mulans: std_logic_vector(22 downto 0);
signal del: std_logic_vector(11 downto 0);
signal flag,sgn,sgn2: std_logic;
signal exp,exp2: std_logic_vector(7 downto 0);
signal code1: std_logic_vector(2 downto 0);

begin

  fetch:invromfetch
    port map(addr,data,data2);

  mul_inv:fmul_inv
    port map(clk,data2,del,flag,mulans);


-- when code1(2 downto 1) = "00" else
     --"11111111110000000000000000000000" when code1 = "111" else
     --sgn & "1111111100000000000000000000000" when code1 < 5 else
     --sgn & "0000000000000000000000000000000";

  process(clk)
    begin
      if rising_edge(clk) then
        del<=op(11 downto 0);
        addr<=op(22 downto 12);
        sgn<=op(31);
        sgn2<=sgn;
        exp<="11111101" - op(30 downto 23);
        exp2<=exp;
        ans<=sgn2 & exp2 & (data - mulans);
		  if addr < 849 then
		   flag<='0';
		  else
		   flag<='1';
		  end if;
      end if;
    end process;
        
end VHDL;
