library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.std_logic_unsigned.all;

--2clkで返す
entity revfadd is
  port(clk:         in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := (others=>'0')
       );
end revfadd;

architecture VHDL of revfadd is

  signal del,exp,exp2 : std_logic_vector(7 downto 0) := (others => '0');
  signal man1,man2,aman1,aman2,man3 : std_logic_vector(24 downto 0);
  signal prior,infer : std_logic_vector(31 downto 0);
  signal sgn,sgn2: std_logic;
  

begin

prior<=op1 when op1(30 downto 0) > op2(30 downto 0) else op2;
infer<=op2 when op1(30 downto 0) > op2(30 downto 0) else op1;
exp<=prior(30 downto 23);
sgn<=prior(31);
del<=prior(30 downto 23) - infer(30 downto 23);
man1<="1" & prior(22 downto 0) & "0";
man2<="1" & infer(22 downto 0) & "0" when del = 0 else
      "01" & infer(22 downto 0) when del = 1 else
      "001" & infer(22 downto 1) when del = 2 else
      "0001" & infer(22 downto 2) when del = 3 else
      "00001" & infer(22 downto 3) when del = 4 else
      "000001" & infer(22 downto 4) when del = 5 else
      "0000001" & infer(22 downto 5) when del = 6 else
      "00000001" & infer(22 downto 6) when del = 7 else
      "000000001" & infer(22 downto 7) when del = 8 else
      "0000000001" & infer(22 downto 8) when del = 9 else
      "00000000001" & infer(22 downto 9) when del = 10 else
      "000000000001" & infer(22 downto 10) when del = 11 else
      "0000000000001" & infer(22 downto 11) when del = 12 else
      "00000000000001" & infer(22 downto 12) when del = 13 else
      "000000000000001" & infer(22 downto 13) when del = 14 else
      "0000000000000001" & infer(22 downto 14) when del = 15 else
      "00000000000000001" & infer(22 downto 15) when del = 16 else
      "000000000000000001" & infer(22 downto 16) when del = 17 else
      "0000000000000000001" & infer(22 downto 17) when del = 18 else
      "00000000000000000001" & infer(22 downto 18) when del = 19 else
      "000000000000000000001" & infer(22 downto 19) when del = 20 else
      "0000000000000000000001" & infer(22 downto 20) when del = 21 else
      "00000000000000000000001" & infer(22 downto 21) when del = 22 else
      "000000000000000000000001" & infer(22) when del = 23 else
      "0000000000000000000000001" when del = 24 else
      "0000000000000000000000000";
  
  pipe:process(clk)
    begin
      if rising_edge(clk) then
        sgn2<=sgn;
        exp2<=exp;
        aman1<=man1;
        aman2<=man2;
      end if;
    end process;

man3<=aman1-aman2;
ans<=sgn2 & exp2 & man3(23 downto 1) when man3(24) = '1' else
     sgn2 & (exp2-1) & man3(22 downto 0) when man3(23) = '1' else
     sgn2 & (exp2-2) & man3(21 downto 0) & "0" when man3(22) = '1' else
     sgn2 & (exp2-3) & man3(20 downto 0) & "00" when man3(21) = '1' else
     sgn2 & (exp2-4) & man3(19 downto 0) & "000" when man3(20) = '1' else
     sgn2 & (exp2-5) & man3(18 downto 0) & "0000" when man3(19) = '1' else
     sgn2 & (exp2-6) & man3(17 downto 0) & "00000" when man3(18) = '1' else
     sgn2 & (exp2-7) & man3(16 downto 0) & "000000" when man3(17) = '1' else
     sgn2 & (exp2-8) & man3(15 downto 0) & "0000000" when man3(16) = '1' else
     sgn2 & (exp2-9) & man3(14 downto 0) & "00000000" when man3(15) = '1' else
     sgn2 & (exp2-10) & man3(13 downto 0) & "000000000" when man3(14) = '1' else
     sgn2 & (exp2-11) & man3(12 downto 0) & "0000000000" when man3(13) = '1' else
     sgn2 & (exp2-12) & man3(11 downto 0) & "00000000000" when man3(12) = '1' else
     sgn2 & (exp2-13) & man3(10 downto 0) & "000000000000" when man3(11) = '1' else
     sgn2 & (exp2-14) & man3(9 downto 0) & "0000000000000" when man3(10) = '1' else
     sgn2 & (exp2-15) & man3(8 downto 0) & "00000000000000" when man3(9) = '1' else
     sgn2 & (exp2-16) & man3(7 downto 0) & "000000000000000" when man3(8) = '1' else
     sgn2 & (exp2-17) & man3(6 downto 0) & "0000000000000000" when man3(7) = '1' else
     sgn2 & (exp2-18) & man3(5 downto 0) & "00000000000000000" when man3(6) = '1' else
     sgn2 & (exp2-19) & man3(4 downto 0) & "000000000000000000" when man3(5) = '1' else
     sgn2 & (exp2-20) & man3(3 downto 0) & "0000000000000000000" when man3(4) = '1' else
     sgn2 & (exp2-21) & man3(2 downto 0) & "00000000000000000000" when man3(3) = '1' else
     sgn2 & (exp2-22) & man3(1 downto 0) & "000000000000000000000" when man3(2) = '1' else
     sgn2 & (exp2-23) & man3(0) & "0000000000000000000000" when man3(1) = '1' else
     sgn2 & (exp2-24) & "00000000000000000000000" when man3(0) = '1' else
     "00000000000000000000000000000000";

end VHDL;
