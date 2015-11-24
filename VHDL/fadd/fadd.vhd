library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返す
entity fadd is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fadd;

architecture VHDL of fadd is

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

  signal absans: std_logic_vector(31 downto 0) := x"00000000";
  signal revans: std_logic_vector(31 downto 0) := x"00000000";
  signal flag,flag2: std_logic := '0';
  signal zflag,zflag2: std_logic := '0';
  signal zop,zop2: std_logic_vector(31 downto 0) := x"00000000";

begin

  chan1:absfadd
    port map(clk,op1,op2,absans);

  chan2:revfadd
    port map(clk,op1,op2,revans);
  
flag<=op1(31) xor op2(31);
zflag<='1' when op1(30 downto 23) = x"00" or op2(30 downto 23) = x"00" else
       '0';
zop<=op2 when op1(30 downto 23) = x"00" else
     op1;

 pipe: process(clk)
 begin
   if rising_edge(clk) then--2clk後に返答
     flag2<=flag;
     zop2<=zop;
     zflag2<=zflag;
   end if;
 end process;
 
ans<=zop2 when zflag2 = '1' else
     absans when flag2='0' else revans;
  
end VHDL;
