library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返す
entity fmul is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmul;

architecture VHDL of fmul is

  signal sgn,asgn:std_logic := '0';
  signal exp1,exp2,exp,aexp:std_logic_vector(7 downto 0);
  signal t11,t12,t13,t21,t22,t23: std_logic_vector(7 downto 0);
  signal r11,r12,r13,r21,r22,r23,r31,r32,r33: std_logic_vector(15 downto 0);
  signal result: std_logic_vector(25 downto 0);

  component octmul -- 8bitで掛け算
  port(clk: std_logic;
       op1,op2:  in std_logic_vector(7 downto 0);
       ans:  out std_logic_vector(15 downto 0)
       );
end component;
  
begin

  mul11:octmul
    port map(clk,t11,t21,r11);
  mul12:octmul
    port map(clk,t11,t22,r12);
  mul13:octmul
    port map(clk,t11,t23,r13);
  mul21:octmul
    port map(clk,t12,t21,r21);
  mul22:octmul
    port map(clk,t12,t22,r22);
  mul23:octmul
    port map(clk,t12,t23,r23);
  mul31:octmul
    port map(clk,t13,t21,r31);
  mul32:octmul
    port map(clk,t13,t22,r32);


t11<="1" & op1(22 downto 16);
t12<=op1(15 downto 8);
t13<=op1(7 downto 0);
t21<="1" & op2(22 downto 16);
t22<=op2(15 downto 8);
t23<=op2(7 downto 0);
  
sgn<=op1(31) xor op2(31);
exp1<=op1(30 downto 23);
exp2<=op2(30 downto 23);
result<=(r11 & "0000000000") +
                   (x"00" & r12 & "00") +
                   (x"00" & r21 & "00") +
                   (x"0000" & r22(15 downto 6)) +
		  (x"0000" & r13(15 downto 6)) +
		  (x"0000" & r31(15 downto 6)) + 
		  (x"000000" & r23(15 downto 14)) +
		  (x"000000" & r32(15 downto 14));



exp<=exp1 + exp2 - 126 when result(25) = '1' else exp1 + exp2 - 127;

ans<=(sgn & exp) & result(24 downto 2) when result(25) = '1' else
     (sgn & exp) & result(23 downto 1);
  
end VHDL;
