library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity fputop is
  port(MCLK1: in std_logic;
       RS_TX: out std_logic);
  end fputop;

architecture VHDL of fputop is

component fadd
  port(clk: in std_logic;
       op1, op2:  in std_logic_vector(31 downto 0);
       ans:       out std_logic_vector(31 downto 0) := x"00000000"
       );
end component;

component u232c
  generic (wtime: std_logic_vector(15 downto 0) := x"1ADB");
  Port ( clk  : in  STD_LOGIC;
         data : in  STD_LOGIC_VECTOR (7 downto 0);
         go   : in  STD_LOGIC;
         busy : out STD_LOGIC;
         tx   : out STD_LOGIC);
end component;

  signal clk,iclk: std_logic;
  type rom_t is array(0 to 497) of std_logic_vector(31 downto 0);
  constant rom: rom_t := ();

  signal op1,op2,ans1,ans,answ,low,high: std_logic_vector(31 downto 0);
  signal addr: std_logic_vector(19 downto 0) := (others=>'0');
  signal miss: std_logic_vector(31 downto 0) := (others=>'0');
  signal rom_o: std_logic_vector(7 downto 0) := (others=>'1');
  signal uart_go: std_logic;
  signal uart_busy: std_logic := '0';
  signal iter: std_logic_vector(31 downto 0) := (others=>'0');
  signal state: std_logic_vector(4 downto 0) := (others=>'0');
  signal state2: std_logic_vector(4 downto 0) := (others=>'0');

begin

  ib: IBUFG port map (
   i=>MCLK1,
   o=>iclk);
  bg: BUFG port map (
    i=>iclk,
    o=>clk);

  fadder:fadd port map
    (clk, op1, op2, ans);

  rs232c: u232c generic map (wtime=>x"1ADB")
  port map (
    clk=>clk,
    data=>rom_o,
    go=>uart_go,
    busy=>uart_busy,
    tx=>rs_tx);


 cal: process(clk)
 begin
   if rising_edge(clk) then-- hoge clk後に返答
     if state = "00000" and addr =  then -- 最後はnop
       state<="00000";
     elsif state = "00000" then
       state<=state+1;
       addr<=addr+3;
       op1<=rom(conv_integer(addr));
       op2<=rom(conv_integer(addr+1));
       ans1<=rom(conv_integer(addr+2));
     elsif state = "00001" then
       state<=state+1;
     elsif state = "00010" then
       state<=state+1;
       answ<=ans;
       state2<="00000";
     elsif state = "00011" and uart_go = '1'then --op1の出力
       state2<=state2+1;
       if op1(31-conv_integer(state2)) = '1' then
         rom_o<=x"31";
       else
         rom_o<=x"30";
       end if;
       if state2 = "11111" then
         state<=state+1;
         state2<="00000";
       end if;
     elsif state = "00100" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "00101" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "00110" and uart_go = '1'then --op2の出力
       state2<=state2+1;
       if op2(31-conv_integer(state2)) = '1' then
         rom_o<=x"31";
       else
         rom_o<=x"30";
       end if;
       if state2 = "11111" then
         state<=state+1;
         state2<="00000";
       end if;
     elsif state = "00111" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "01000" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "01001" and uart_go = '1'then --outputの出力
       state2<=state2+1;
       if answ(31-conv_integer(state2)) = '1' then
         rom_o<=x"31";
       else
         rom_o<=x"30";
       end if;
       if state2 = "11111" then
         state<=state+1;
         state2<="00000";
       end if;
     elsif state = "01010" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "01011" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "01100" and uart_go = '1'then --answerの出力
       state2<=state2+1;
       if ans1(31-conv_integer(state2)) = '1' then
         rom_o<=x"31";
       else
         rom_o<=x"30";
       end if;
       if state2 = "11111" then
         state<=state+1;
         state2<="00000";
       end if;
     elsif state = "01101" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "01110" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "01111" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "10000" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "10001" then --low,highとの比較
       state<=state+1;
       low<=ans1 - 5;
     elsif state = "10010" then
       state<=state+1;
       high<=ans1 + 5;
     elsif state = "10011" then
       iter<=iter+1;
       state<=state+1;
       if high < answ or low > answ then
         miss<=miss+1;
       end if;
     elsif state = "10100" and uart_go = '1'then --iterの出力
       state2<=state2+1;
       if iter(31-conv_integer(state2)) = '1' then
         rom_o<=x"31";
       else
         rom_o<=x"30";
       end if;
       if state2 = "11111" then
         state<=state+1;
         state2<="00000";
       end if;
     elsif state = "10101" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "10110" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "10111" and uart_go = '1'then --missの出力
       state2<=state2+1;
       if miss(31-conv_integer(state2)) = '1' then
         rom_o<=x"31";
       else
         rom_o<=x"30";
       end if;
       if state2 = "11111" then
         state<=state+1;
         state2<="00000";
       end if;
     elsif state = "11000" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "11001" and uart_go = '1'then
       state<=state+1;
       rom_o<=x"0a";
     elsif state = "11010" and uart_go = '1'then -- 改行
       state<=state+1;
       rom_o<=x"0d";
     elsif state = "11011" and uart_go = '1'then
       state<="00000";
       rom_o<=x"0a";
     end if;
   end if;
 end process;

    send_msg: process(clk)
  begin
    if rising_edge(clk) then
      if uart_busy='0' and uart_go='0' then
        uart_go<='1';
      else
        uart_go<='0';
      end if;
    end if;
  end process;
  
end VHDL;
