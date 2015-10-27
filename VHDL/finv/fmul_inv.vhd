library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

entity fmul_inv is
  port(clk: in std_logic;
       romdata:  in std_logic_vector(22 downto 0); --23bit
       del: in std_logic_vector(11 downto 0); --12bit
       flag: in std_logic;
       ans:  out std_logic_vector(22 downto 0) -- dataと引き算するため23bit
       );
end fmul_inv;

architecture VHDL of fmul_inv is
  signal preans,preans2: std_logic_vector(12 downto 0);
  signal d1,d2,d3,de1,de2,de3,ad1,ad2,ad3,ade1,ade2,ade3: std_logic_vector(3 downto 0);
  signal r11,r12,r21,r22,r13,r31: std_logic_vector(7 downto 0);
  signal flag2: std_logic;

begin
  
d1<=romdata(22 downto 19);
d2<=romdata(18 downto 15);
d3<=romdata(14 downto 11);
de1<=del(11 downto 8);
de2<=del(7 downto 4);
de3<=del(3 downto 0);

  pipe:process(clk)
  begin
    if rising_edge(clk) then
     flag2<=flag;
     preans2<=preans;
	  --ad1<=d1;
	  --ad2<=d2;
	  --ad3<=d3;
	  --ade1<=de1;
	  --ade2<=de2;
	  --ade3<=de3;
	  r11<=d1*de1;
	  r12<=d1*de2;
	  r21<=d2*de1;
     r22<=d2*de2;
     r13<=d1*de3;
     r31<=d3*de1;
    end if;
  end process;
  
--r11<=ad1*ade1;
--r12<=ad1*ade2;
--r21<=ad2*ade1;
--r22<=ad2*ade2;
--r13<=ad1*ade3;
--r31<=ad3*ade1;  


preans<=("0" & del) + ("0" & r11 & "0000") + ("00000" & r12) + ("00000" & r21) + ("000000000" & r22(7 downto 4)) + ("00000000" & r13(7 downto 4)) + ("00000000" & r31(7 downto 4));

ans<="0000000000" & (preans2 + 1) when flag2='0' else
     "00000000000" & preans2(12 downto 1);
  
  end VHDL;
