library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 3clkで答を返す
entity fmul is
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmul;

architecture VHDL of fmul is

  signal sgn,asgn,aasgn:std_logic := '0';
  signal exp1,exp2,exp,aexp,aaexp:std_logic_vector(7 downto 0);
  signal t11,t12,t13,t21,t22,t23: std_logic_vector(7 downto 0);
  signal r11,r12,r13,r21,r22,r23,r31,r32: std_logic_vector(15 downto 0);
  signal ar13,ar31,ar23,ar32: std_logic_vector(25 downto 0);
  signal result,aresult,aresult2: std_logic_vector(25 downto 0);
  signal zflag,zflag2,zflag3: std_logic;
  
begin


t11<="1" & op1(22 downto 16);
t12<=op1(15 downto 8);
t13<=op1(7 downto 0);
t21<="1" & op2(22 downto 16);
t22<=op2(15 downto 8);
t23<=op2(7 downto 0);

sgn<=op1(31) xor op2(31);
exp1<=op1(30 downto 23);  
exp2<=op2(30 downto 23);
exp<=exp1 + exp2 - 127;

zflag<='1' when op1(30 downto 23) = x"00" or op2(30 downto 23) = x"00" else
       '0';

  pipe:process(clk)
    begin
      if rising_edge(clk) then
       r11<=t11*t21;
       r12<=t11*t22;
       r21<=t12*t21;
       r22<=t12*t22;
       r31<=t13*t21;
       r13<=t11*t23;
       r23<=t12*t23;
       r32<=t13*t22;
       aexp<=exp;
       asgn<=sgn; 
       zflag2<=zflag;
      end if;
    end process;

result<=(r11 & "0000000000") +
        (x"00" & r12 & "00") +
        (x"00" & r21 & "00") +
        (x"0000" & r22(15 downto 6));

    pipe2:process(clk)
    begin
      if rising_edge(clk) then
        aresult<=result;
        ar13<=x"0000" & r13(15 downto 6);
        ar31<=x"0000" & r31(15 downto 6);
        ar23<=x"000000" & r23(15 downto 14);
        ar32<=x"000000" & r32(15 downto 14);
        aaexp<=aexp;
        aasgn<=asgn;
        zflag3<=zflag2;
      end if;
    end process;

aresult2<=aresult+ar13+ar31+ar23+ar32;

ans<= x"00000000" when zflag3='1' else
     aasgn & (aaexp+1) & aresult2(24 downto 2) when aresult2(25) = '1' else
     aasgn & aaexp & aresult2(23 downto 1);
      
end VHDL;
