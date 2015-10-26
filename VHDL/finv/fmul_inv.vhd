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
  signal preans: std_logic_vector(12 downto 0);
  signal d1,d2,d3,de1,de2,de3: std_logic_vector(3 downto 0);
  signal r11,r12,r21,r22,r13,r31: std_logic_vector(7 downto 0);

  component quamul
  port(clk:       in std_logic;
       op1, op2:  in std_logic_vector(3 downto 0);
       ans:       out std_logic_vector(7 downto 0) := x"00"
       );
end component;

begin

  mul1:quamul
    port map(clk,d1,de1,r11);
  mul2:quamul
    port map(clk,d1,de2,r12);
  mul3:quamul
    port map(clk,d2,de1,r21);
  mul4:quamul
    port map(clk,d2,de2,r22);
  mul5:quamul
    port map(clk,d1,de3,r13);
  mul6:quamul
    port map(clk,d3,de1,r31);
  
  d1<=romdata(22 downto 19);
  d2<=romdata(18 downto 15);
  d3<=romdata(14 downto 11);
  de1<=del(11 downto 8);
  de2<=del(7 downto 4);
  de3<=del(3 downto 0);
preans<=("0" & del) + ("0" & r11 & "0000") + ("00000" & r12) + ("00000" & r21) + ("000000000" & r22(7 downto 4)) + ("00000000" & r13(7 downto 4)) + ("00000000" & r31(7 downto 4));

	process(clk)
	 begin
	  if rising_edge(clk) then
	    if flag = '0' then
			ans<= "0000000000" & (preans + 1);
		 else
			ans<="00000000000" & preans(12 downto 1);
		 end if;
	  end if;
	end process;


  end VHDL;