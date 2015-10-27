library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

entity fmul_sqrt is
  port(clk: in std_logic;
       romdata:  in std_logic_vector(22 downto 0); --23bit
       del: in std_logic_vector(12 downto 0); --13bit
       flag: in std_logic;
       ans:  out std_logic_vector(22 downto 0) -- dataと引き算するため23bit
       );
end fmul_sqrt;

architecture VHDL of fmul_sqrt is

  signal flag2: std_logic;
  signal preans: std_logic_vector(13 downto 0);
  signal d1,d2,d3: std_logic_vector(3 downto 0);
  signal de1: std_logic_vector(4 downto 0);
  signal de2,de3: std_logic_vector(3 downto 0);
  signal r12,r22,r13: std_logic_vector(7 downto 0);
  signal r11,r21,r31: std_logic_vector(8 downto 0);
  signal del2: std_logic_vector(12 downto 0);

begin
  
d1<=romdata(22 downto 19);
d2<=romdata(18 downto 15);
d3<=romdata(14 downto 11);
de1<=del(12 downto 8);
de2<=del(7 downto 4);
de3<=del(3 downto 0); 
  
 pipe:process(clk)
  begin
   if rising_edge(clk) then
     flag2<=flag;
	  del2<=del;
     r11<=d1*de1;
     r12<=d1*de2;
     r21<=d2*de1;
     r22<=d2*de2;
     r13<=d1*de3;
     r31<=d3*de1;
   end if;
 end process;

 preans<=("0" & del2) + ("0" & r11 & "0000") + ("000000" & r12) + ("00000" & r21) + ("0000000000" & r22(7 downto 4)) + ("0000000000" & r13(7 downto 4)) + ("000000000" & r31(8 downto 4));

 ans<="0000000000" & preans(13 downto 1) + 2 when flag2 = '0' else
      "00000000000" & preans(13 downto 2);

end VHDL;
