library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

entity revfadd is
  port(clk:         in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := (others=>'0')
       );
end revfadd;

architecture VHDL of revfadd is
component round2
  port(op: in std_logic_vector(31 downto 0);
       point: in std_logic_vector(24 downto 0);
       ans: out std_logic_vector(31 downto 0));
  end component;

  signal rouans : std_logic_vector(31 downto 0);
  signal del : std_logic_vector(8 downto 0) := (others => '0');
  signal op : std_logic_vector(31 downto 0) := (others=>'0');
  signal point,pointpre : std_logic_vector(24 downto 0) := (others=>'0');
  signal man1,man2 : std_logic_vector(24 downto 0) := (others=>'0');
  constant zeroes : std_logic_vector(24 downto 0) := (others=>'0');
  signal result :  std_logic_vector(24 downto 0) := (others=>'0');
  signal prior,infor : std_logic_vector(31 downto 0) := (others=>'0');

begin

  round:round2
    port map(op,point,rouans);

ans<=rouans;

del<="0" & (op1(30 downto 23) - op2(30 downto 23)) when op1(30 downto 23) > op2(30 downto 23) else
     "1" & (op2(30 downto 23) - op1(30 downto 23)) when op2(30 downto 23) > op1(30 downto 23) else
     "000000000" when op1(22 downto 0) > op2(22 downto 0) else
     "100000000";
prior<=op1 when del(8) = '0' else op2;
infor<=op2 when del(8) = '0' else op1;
man1<="1" & prior(22 downto 0) & "0";
man2<=zeroes when del(7 downto 0) > "00011000" else
      zeroes(23 downto 0) & infor(22) when del(7 downto 0) = "00011000" and infor(30 downto 23) = "00000000" else
      infor(22 downto 0) & "00" when del(7 downto 0) = "00000000" and infor(30 downto 23) = "00000000" else
      "0" & infor(22 downto 0) & "0" when del(7 downto 0) = "00000001" and infor(30 downto 23) = "00000000" else
      zeroes(((conv_integer(del(7 downto 0)))-1) downto 0)& infor(22 downto (conv_integer(del(7 downto 0))-2)) when infor(30 downto 23) = "00000000" else
      zeroes(23 downto 0) & "1" when del(7 downto 0) = "00011000" else
      zeroes(22 downto 0) & "1" & infor(22) when del(7 downto 0) = "00010111" else
      "1" & infor(22 downto 0) & "0" when del(7 downto 0) = "00000000" else
      "01" & infor(22 downto 0) when del(7 downto 0) = "00000001" else
      zeroes(((conv_integer(del(7 downto 0)))-1) downto 0) & "1" & infor(22 downto (conv_integer(del(7 downto 0)) - 1));
result<=man1-man2;
pointpre<=zeroes when del(7 downto 0) > "00011000" or del(7 downto 0) < x"02" else
          zeroes when infor(30 downto 23) = "00000000" and del(7 downto 0) < "00000011" else
          infor(0) & zeroes(23 downto 0) when infor(30 downto 23) = "00000000" and del(7 downto 0) = "00000011" else
          infor(((conv_integer(del(7 downto 0)))-3) downto 0) & zeroes((26-(conv_integer(del(7 downto 0)))) downto 0) when infor(30 downto 23) = "00000000" else
          infor(0) & zeroes(23 downto 0) when del(7 downto 0) = "00000010" else
          infor(((conv_integer(del(7 downto 0)))-2) downto 0) & zeroes((25-(conv_integer(del(7 downto 0)))) downto 0);
  
  process(clk)begin--round
    if rising_edge(clk) then
      if result(24 downto 0) = "0000000000000000000000000" then
        op<=x"00000000";
        point<="0000000000000000000000000";
      elsif result(24 downto 0) = "0000000000000000000000001" then
        if prior(30 downto 23) > "00011000" then
          op<=(prior(31 downto 23) - 24) & zeroes(22 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 1) = "000000000000000000000001" then
        if prior(30 downto 23) > "00010111" then
          op<=(prior(31 downto 23) - 23) & result(0) & zeroes(21 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 2) = "00000000000000000000001" then
        if prior(30 downto 23) > "00010110" then
          op<=(prior(31 downto 23) - 22) & result(1 downto 0) & zeroes(20 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 3) = "0000000000000000000001" then
        if prior(30 downto 23) > "00010101" then
          op<=(prior(31 downto 23) - 21) & result(2 downto 0) & zeroes(19 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 4) = "000000000000000000001" then
        if prior(30 downto 23) > "00010100" then
          op<=(prior(31 downto 23) - 20) & result(3 downto 0) & zeroes(18 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 5) = "00000000000000000001" then
        if prior(30 downto 23) > "00010011" then
          op<=(prior(31 downto 23) - 19) & result(4 downto 0) & zeroes(17 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 6) = "0000000000000000001" then
        if prior(30 downto 23) > "00010010" then
          op<=(prior(31 downto 23) - 18) & result(5 downto 0) & zeroes(16 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 7) = "000000000000000001" then
        if prior(30 downto 23) > "00010001" then
          op<=(prior(31 downto 23) - 17) & result(6 downto 0) & zeroes(15 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 8) = "00000000000000001" then
        if prior(30 downto 23) > "00010000" then
          op<=(prior(31 downto 23) - 16) & result(7 downto 0) & zeroes(14 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 9) = "0000000000000001" then
        if prior(30 downto 23) > "00001111" then
          op<=(prior(31 downto 23) - 15) & result(8 downto 0) & zeroes(13 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 10) = "000000000000001" then
        if prior(30 downto 23) > "00001110" then
          op<=(prior(31 downto 23) - 14) & result(9 downto 0) & zeroes(12 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 11) = "00000000000001" then
        if prior(30 downto 23) > "00001101" then
          op<=(prior(31 downto 23) - 13) & result(10 downto 0) & zeroes(11 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 12) = "0000000000001" then
        if prior(30 downto 23) > "00001100" then
          op<=(prior(31 downto 23) - 12) & result(11 downto 0) & zeroes(10 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 13) = "000000000001" then
        if prior(30 downto 23) > "00001011" then
          op<=(prior(31 downto 23) - 11) & result(12 downto 0) & zeroes(9 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 14) = "00000000001" then
        if prior(30 downto 23) > "00001010" then
          op<=(prior(31 downto 23) - 10) & result(13 downto 0) & zeroes(8 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 15) = "0000000001" then
        if prior(30 downto 23) > "00001001" then
          op<=(prior(31 downto 23) - 9) & result(14 downto 0) & zeroes(7 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 16) = "000000001" then
        if prior(30 downto 23) > "00001000" then
          op<=(prior(31 downto 23) - 8) & result(15 downto 0) & zeroes(6 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 17) = "00000001" then
        if prior(30 downto 23) > "00000111" then
          op<=(prior(31 downto 23) - 7) & result(16 downto 0) & zeroes(5 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 18) = "0000001" then
        if prior(30 downto 23) > "00000110" then
          op<=(prior(31 downto 23) - 6) & result(17 downto 0) & zeroes(4 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 19) = "000001" then
        if prior(30 downto 23) > "00000101" then
          op<=(prior(31 downto 23) - 5) & result(18 downto 0) & zeroes(3 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 20) = "00001" then
        if prior(30 downto 23) > "00000100" then
          op<=(prior(31 downto 23) - 4) & result(19 downto 0) & zeroes(2 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 21) = "0001" then
        if prior(30 downto 23) > "00000011" then
          op<=(prior(31 downto 23) - 3) & result(20 downto 0) & zeroes(1 downto 0);
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 22) = "001" then
        if prior(30 downto 23) > "00000010" then
          op<=(prior(31 downto 23) - 2) & result(21 downto 0) & "0";
          point<="0000000000000000000000000";
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(24 downto 23) = "01" then
        if prior(30 downto 23) > "00000001" then
          op<=(prior(31 downto 23) - 1) & result(22 downto 0);
          point<=pointpre;
        else
          op<=prior(31) & "00000000" & result(23 downto 1);
          point<=result(0) & zeroes(23 downto 0);
        end if;
      elsif result(0) = '0' then
        op<=prior(31 downto 23) & result(23 downto 1);
        point<="0" & pointpre(24 downto 1);
      else
        op<=prior(31 downto 23) & (result(23 downto 1) + 1);
        point<="1" & pointpre(24 downto 1);
      end if;
    end if;
  end process;

end VHDL;
