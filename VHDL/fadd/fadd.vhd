library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- clkで答を返す
entity fadd is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fadd;

architecture VHDL of fadd is

component encode -- opがinf,nan,非正規化数,0かcheck
  port(op:  in std_logic_vector(31 downto 0);
       ans:  out std_logic_vector(2 downto 0)
       );
end component;

component infcal
  port(op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0)
       );
end component;

component non_norm -- nonnorm+nonnorm
  port(op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0)
       );
end component;

component absfadd
  port(clk:       in std_logic;
       op1,op2: in std_logic_vector(31 downto 0);
       ans: out std_logic_vector(31 downto 0));
  end component;

component revfadd
  port(clk:       in std_logic;
       op1,op2: in std_logic_vector(31 downto 0);
       ans: out std_logic_vector(31 downto 0));
  end component;

  signal flag1: std_logic_vector(1 downto 0) := "00";
  signal flag2: std_logic_vector(1 downto 0) := "00";
  signal infans: std_logic_vector(31 downto 0) := x"00000000";
  signal infresult1: std_logic_vector(31 downto 0) := x"00000000";
  signal op3,op4: std_logic_vector(31 downto 0) := x"00000000";
  signal absans: std_logic_vector(31 downto 0) := x"00000000";
  signal revans: std_logic_vector(31 downto 0) := x"00000000";
  signal nnans,nnresult: std_logic_vector(31 downto 0) := x"00000000";
  signal sign1,sign2: std_logic := '0';
  signal opc1,opc2,opc3,opc4: std_logic_vector(2 downto 0) := "000";

begin

  opch1:encode
    port map(op1,opc1);

  opch2:encode
    port map(op2,opc2);

  infcalc:infcal -- 1clk後
    port map(op1,op2,infans);

  nonnormcalc:non_norm -- 1clk後
    port map(op1,op2,nnans);

  chan1:absfadd
    port map(clk,op1,op2,absans);

  chan2:revfadd
    port map(clk,op1,op2,revans);
  
ans<=op3 when opc4="101" else -- any + zero
     op4 when opc3="101" else -- zero + any
     nnresult when opc3(1 downto 0)="10" and opc4(1 downto 0)="10" else -- nonnorm + nonnorm
     absans when (opc3(2 downto 1) = "01" and opc4(2 downto 1) = "01") or (opc3(2 downto 1) = "11" and opc4(2 downto 1) = "11") else
     revans when (opc3(2 downto 1) = "01" and opc4(2 downto 1) = "11") or (opc3(2 downto 1) = "11" and opc4(2 downto 1) = "01") else
     infresult1;
  
 loopcal: process(clk)
 begin
   if rising_edge(clk) then--2clk後に返答
     opc3<=opc1;
     opc4<=opc2;
     op3<=op1;
     op4<=op2;
     nnresult<=nnans;
     infresult1<=infans;
   end if;
 end process;
  
end VHDL;
