library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 2clkで答を返す
entity fmulsub is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmulsub;

architecture VHDL of fmulsub is
  signal sgn:std_logic := '0';
  signal exp1,exp2,exp:std_logic_vector(7 downto 0);
  signal result: std_logic_vector(46 downto 0) := (others=>'0');
  signal t11,t12,t13,t21,t22,t23: std_logic_vector(7 downto 0)
  signal r11,r12,r13,r21,r22,r23,r31,r32,r33: std_logic_vector(15 downto 0);
  signal result: std_logic_vector(23 downto 0);

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
  mul33:octmul
    port map(clk,t13,t23,r33);

  
sgn<=op1(31) xor op2(31);
exp1<=op1(30 downto 23);
exp2<=op2(30 downto 23);

result<=(x"00" & (r11 & x"00")) +
        (x"0000" & r12) +
        (x"0000" & r21) +
        (x"000000" & r22(15 downto 8));

exp<=exp1 + exp2 - 127 when result(24) = '1' else exp1 + exp2 - 126;

ans<=(sgn & exp) & result(23 downto 1) when result(24) = '1' else
     (sgn & exp) & result(22 downto 0);
  
end VHDL;