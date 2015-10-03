library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.std_logic_textio.all;
use std.textio.all;  

library UNISIM;
use UNISIM.VComponents.all;

entity loopback is
  generic (wtime : std_logic_vector(15 downto 0) := x"1B16";
           wtime2 : std_logic_vector(15 downto 0) := x"1C07");
  --generic (wtime : std_logic_vector(15 downto 0) := x"0003");
  Port ( MCLK1 : in  STD_LOGIC;
         --clk: in std_logic;
         RS_TX : out  STD_LOGIC := '1';
         RS_RX : in STD_LOGIC);
  end loopback;

architecture RTL of loopback is
  signal clk,iclk: std_logic;
  signal sendbuf: std_logic_vector(8 downto 0) := (others=>'1');
  signal countdown: std_logic_vector(15 downto 0) := (others=>'0');
  signal countdown2: std_logic_vector(15 downto 0) := (others=>'0');
  signal state: std_logic_vector(3 downto 0) := "1111";
  signal state2: std_logic_vector(3 downto 0) := "1111";
  signal data: std_logic_vector(7 downto 0) := "11111111";
  signal data2: std_logic_vector(7 downto 0) := "11111111";
  signal go: std_logic;
begin
  ib: IBUFG port map (
    i=>MCLK1,
    o=>iclk);
  bg: BUFG port map (
    i=>iclk,
    o=>clk);
  
  sendmsg: process(clk) --送信プロセス
  begin
    if rising_edge(clk) then
      case state is
        when "1111"=> --wait state
          if go='1' then --goが1&&stateが1011のとき送信開始
            sendbuf<=data&"0";
            state<=state-1;
            countdown<=wtime;
          end if;
        when "0110"=>
          if countdown=0 then --stopbitを送ったらwait stateへ
            sendbuf<="1"&sendbuf(8 downto 1);
            countdown<=wtime;
            state<="1111";
          else
            countdown<=countdown-1; --1clk毎にデクリメント
          end if;
        when others=>
          if countdown=0 then --wtime*1clktime経てば1bit送る
            sendbuf<="1"&sendbuf(8 downto 1);
            countdown<=wtime;
            state<=state-1;
          else
            countdown<=countdown-1; --1clk毎にデクリメント
          end if;
      end case;
    end if;
  end process;
  rs_tx<=sendbuf(0);

  recvmsg: process(clk) --受信プロセス
    variable latch : std_logic := '1';
  begin
    if rising_edge(clk) then
      case state2 is
        when "1111"=> --wait state
          go<='0';
          if rs_rx='0' then --0を受け取ったとき受信開始
            state2<=state2-1;
            countdown2<=wtime2+2763;
          end if;
        when "0110"=> --stopbitを受信したらwait stateへ
          if countdown2=0 then
            go<='1';
            data<=data2;
            countdown2<=wtime2;
            state2<="1111";
          else
            countdown2<=countdown2-1; --1clk毎にデクリメント
          end if;
        when others=>
          latch := rs_rx; --latchで受ける
          if countdown2=0 then --wtime*1clktime経てば1bit受信
            data2<=latch & data2(7 downto 1);
            countdown2<=wtime2;
            state2<=state2-1;
          else
            countdown2<=countdown2-1; --1clk毎にデクリメント
          end if;
      end case;
    end if;
  end process;

          
end RTL;
