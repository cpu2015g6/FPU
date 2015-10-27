library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.std_logic_unsigned.all;

--2clkで返す
entity absfadd is
  port(clk:         in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end absfadd;

architecture VHDL of absfadd is
  
  signal sgn,sgn2: std_logic;
  signal del,exp,exp2: std_logic_vector(7 downto 0) := (others => '0');
  signal man1,man2,aman1,aman2,man3 : std_logic_vector(25 downto 0) := (others=>'0');
  signal prior,infer: std_logic_vector(31 downto 0) := (others=>'0');

begin

sgn<=op1(31);
prior<=op1 when op1(30 downto 0) > op2(30 downto 0) else op2;
infer<=op2 when op1(30 downto 0) > op2(30 downto 0) else op1;
exp<=prior(30 downto 23);
del<=prior(30 downto 23) - infer(30 downto 23);
man1<="01" & prior(22 downto 0) & "0";
man2<="01" & infer(22 downto 0) & "0" when del = 0 else
      "001" & infer(22 downto 0) when del = 1 else
      "0001" & infer(22 downto 1) when del = 2 else
      "00001" & infer(22 downto 2) when del = 3 else
      "000001" & infer(22 downto 3) when del = 4 else
      "0000001" & infer(22 downto 4) when del = 5 else
      "00000001" & infer(22 downto 5) when del = 6 else
      "000000001" & infer(22 downto 6) when del = 7 else
      "0000000001" & infer(22 downto 7) when del = 8 else
      "00000000001" & infer(22 downto 8) when del = 9 else
      "000000000001" & infer(22 downto 9) when del = 10 else
      "0000000000001" & infer(22 downto 10) when del = 11 else
      "00000000000001" & infer(22 downto 11) when del = 12 else
      "000000000000001" & infer(22 downto 12) when del = 13 else
      "0000000000000001" & infer(22 downto 13) when del = 14 else
      "00000000000000001" & infer(22 downto 14) when del = 15 else
      "000000000000000001" & infer(22 downto 15) when del = 16 else
      "0000000000000000001" & infer(22 downto 16) when del = 17 else
      "00000000000000000001" & infer(22 downto 17) when del = 18 else
      "000000000000000000001" & infer(22 downto 18) when del = 19 else
      "0000000000000000000001" & infer(22 downto 19) when del = 20 else
      "00000000000000000000001" & infer(22 downto 20) when del = 21 else
      "000000000000000000000001" & infer(22 downto 21) when del = 22 else
      "0000000000000000000000001" & infer(22) when del = 23 else
      "00000000000000000000000001" when del = 24 else
      "00000000000000000000000000";

  pipe:process(clk)
   begin
    if rising_edge(clk) then
      aman1<=man1;
      aman2<=man2;
      exp2<=exp;
      sgn2<=sgn;
    end if;
  end process;

man3<=aman1+aman2;
ans<=sgn2 & exp2 & man3(23 downto 1) when man3(25) = '0' else sgn2 & (exp2+1) & man3(24 downto 2);

end VHDL;
