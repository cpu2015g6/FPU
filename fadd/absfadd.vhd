library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

entity absfadd is
  port(clk:         in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end absfadd;

architecture VHDL of absfadd is
component round1
  port(op : in std_logic_vector(31 downto 0);
       point : in std_logic_vector(24 downto 0);
       ans: out std_logic_vector(31 downto 0));
  end component;

  signal rouans: std_logic_vector(31 downto 0) := (others=>'0');
  signal del : std_logic_vector(8 downto 0) := (others => '0');
  signal op,oppre : std_logic_vector(31 downto 0) := (others=>'0');
  signal point,pointpre : std_logic_vector(24 downto 0) := (others=>'0');
  signal man1,man2 : std_logic_vector(24 downto 0) := (others=>'0');
  constant zeroes : std_logic_vector(24 downto 0) := (others=>'0');
  signal result :  std_logic_vector(24 downto 0) := (others=>'0');
  signal prior,infor: std_logic_vector(31 downto 0) := (others=>'0');

begin

  round:round1
    port map(op,point,rouans);

ans<=rouans;
  
del<="0" & (op1(30 downto 23) - op2(30 downto 23)) when op1(30 downto 23) > op2(30 downto 23) else
     "1" & (op2(30 downto 23) - op1(30 downto 23));
prior<=op1 when del(8) = '0' else op2;
infor<=op1 when del(8) = '1' else op2;
man1<="01" & prior(22 downto 0);
man2<=zeroes when del(7 downto 0) >= "00011000" else
      zeroes(23 downto 0) & infor(22) when del(7 downto 0) = "00010111" and infor(30 downto 23) = "00000000" else
      "0" & infor(22 downto 0) & "0" when del(7 downto 0) = "00000000" and infor(30 downto 23) = "00000000" else
      zeroes(conv_integer(del(7 downto 0)) downto 0) & infor(22 downto (conv_integer(del(7 downto 0))-1)) when infor(30 downto 23) = "00000000" else
      zeroes(23 downto 0) & "1" when del(7 downto 0) = "00010111" else
      zeroes(22 downto 0) & "1" & infor(22) when del(7 downto 0) = "00010110" else
      "01" & infor(22 downto 0) when del(7 downto 0) = "00000000" else
      zeroes((conv_integer(del(7 downto 0))) downto 0) & "1" & infor(22 downto (conv_integer(del(7 downto 0))));
      
pointpre<=zeroes when del(7 downto 0) > "00011000" or del(7 downto 0) = "00000000" else
          infor(0) & zeroes(23 downto 0) when del(7 downto 0) = "00000010" and infor(30 downto 23) = "00000000" else
          zeroes when del(7 downto 0) < "00000010" and infor(30 downto 23) = "00000000" else
          infor(((conv_integer(del(7 downto 0)))-2) downto 0) & zeroes((25-(conv_integer(del(7 downto 0)))) downto 0) when infor(30 downto 23) = "00000000" else
          "1" & infor(22 downto 0) & "0" when del(7 downto 0) = "00011000" else
          infor(0) & zeroes(23 downto 0) when del(7 downto 0) = "00000001" else
          infor(((conv_integer(del(7 downto 0)))-1) downto 0) & zeroes((24-(conv_integer(del(7 downto 0)))) downto 0);

result<=man1+man2;
oppre<=(prior(31 downto 23) + 1) & result(23 downto 1) when result(24) = '1' else
       prior(31 downto 23) & result(22 downto 0) when del(8) = '0' else
       (prior(31 downto 23) + 1) & result(23 downto 1) when result(24) = '1' else
       prior(31 downto 23) & result(22 downto 0) when del(8) = '1';
  
  process(clk)begin--round
    if rising_edge(clk) then
      if oppre(30 downto 0) >= "1111111100000000000000000000000" then
        op<=oppre(31 downto 23) & zeroes(22 downto 0);
        point<=zeroes;
      elsif result(24) = '1' then
        op<=oppre;
        point<=result(0) & pointpre(24 downto 1);
      else
        op<=oppre;
        point<=pointpre;
      end if;
    end if;
  end process;

end VHDL;
