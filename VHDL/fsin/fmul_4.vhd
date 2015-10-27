library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

-- 1clkで答を返す,必ず正でpi/4以下のものがくる
entity fmul_3 is
  generic(const: std_logic_vector(23 downto 0) := x"aaa789");
  port(clk:       in std_logic;
       op:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end fmul_3;

architecture VHDL of fmul_3 is
  signal r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14;
  signal pre: std_logic_vector(27 downto 0);
  signal exp,exp2: std_logic_vector(7 downto 0);

begin
r1<="0" & const & "000";
r2<="00" & const & "00" when op(22) = '1' else x"0000000";
r3<="000" & const & "0" when op(21) = '1' else x"0000000";
r4<="0000" & const  when op(21) = '1' else x"0000000";
r5<="00000" & const(23 downto 1) when op(21) = '1' else x"0000000";
r6<="000000" & const(23 downto 2) when op(21) = '1' else x"0000000";
r7<="0000000" & const(23 downto 3) when op(21) = '1' else x"0000000";
r8<="00000000" & const(23 downto 4) when op(21) = '1' else x"0000000";
r9<="000000000" & const(23 downto 5) when op(21) = '1' else x"0000000";
r10<="0000000000" & const(23 downto 6) when op(21) = '1' else x"0000000";
r11<="00000000000" & const(23 downto 7) when op(21) = '1' else x"0000000";
r12<="000000000000" & const(23 downto 8) when op(21) = '1' else x"0000000";
r13<="0000000000000" & const(23 downto 9) when op(21) = '1' else x"0000000";
r14<="00000000000000" & const(23 downto 10) when op(21) = '1' else x"0000000";
r15<="000000000000000" & const(23 downto 11) when op(21) = '1' else x"0000000";
r16<="0000000000000000" & const(23 downto 12) when op(21) = '1' else x"0000000";
r17<="00000000000000000" & const(23 downto 13) when op(21) = '1' else x"0000000";
r18<="000000000000000000" & const(23 downto 14) when op(21) = '1' else x"0000000";
r19<="0000000000000000000" & const(23 downto 15) when op(21) = '1' else x"0000000";
r20<="00000000000000000000" & const(23 downto 16) when op(21) = '1' else x"0000000";
r21<="000000000000000000000" & const(23 downto 17) when op(21) = '1' else x"0000000";
r22<="0000000000000000000000" & const(23 downto 18) when op(21) = '1' else x"0000000";
r23<="00000000000000000000000" & const(23 downto 19) when op(21) = '1' else x"0000000";
r24<="000000000000000000000000" & const(23 downto 20) when op(21) = '1' else x"0000000";

pre<=r1+r2+r3+r4+r5+r6+r7+r8+r9+r10+r11+r12+r13+r14+r15+r16+r17+r18+r19+r20+r21+r22+r23+r24;

exp<=op(30 downto 23) - 4 when pre(27) = '1' else op(30 downto 23) - 5;

ans<="0" & exp & pre(26 downto 4) when pre(27) = '1' else "0" & exp & pre(25 downto 3); 

end VHDL;
 
