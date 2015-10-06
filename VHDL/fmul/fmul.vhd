library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返す
entity fmul is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmul;

architecture VHDL of fmul is

component encode -- opのstatusをencode
  port(op:  in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(2 downto 0)
       );
end component;

  signal opc1,opc2,opc3,opc4: std_logic_vector(2 downto 0) := "000";

begin

  opch1:encode
    port map(op1,opc1);

  opch2:encode
    port map(op2,opc2);

  mul:fmulsub -- mul norm and norm
    port map(clk,op1,op2,ans);
  
ans<=rouans when opc3 < "100" and opc4 < "100" else -- 正規化数、非正規化数の掛け算
     "11111111110000000000000000000000" when opc3 = "111" or opc4 = "111" or opc3 > "101" and opc4 > "101" and opc3 xor opc4 /= "000" else -- NaN
     "00000000000000000000000000000000" when opc3 = "110" or opc4 = "110" else -- either op1 or op2 is zero
     "01111111100000000000000000000000" when (sig1 xor sig2 = '0') and ("0" & op1(30 downto 23)) + ("0" & op2(30 downto 23)) > "110000000" else -- +Inf
     "11111111100000000000000000000000"; -- -Inf

  
 loopcal: process(clk)
 begin
   if rising_edge(clk) then--2clk後に返答するためstatusをenqueue
     opc3<=opc1;
     opc4<=opc2;
   end if;
 end process;
  
end VHDL;
