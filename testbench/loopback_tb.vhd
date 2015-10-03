library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use ieee.std_logic_textio.all;
use std.textio.all;  

library UNISIM;
use UNISIM.VComponents.all;

entity loopback_tb is
  generic (wtime : std_logic_vector(15 downto 0) := x"1B16");
  --generic (wtime : std_logic_vector(15 downto 0) := x"0003");
end loopback_tb;

architecture testbench of loopback_tb is

  signal clk: std_logic := '0';
  signal tx: std_logic := '0';
  signal rx: std_logic :='0';
  signal sendbuf: std_logic_vector(8 downto 0) := (others=>'1');
  signal recvbuf: std_logic_vector(7 downto 0) := (others=>'1');
  signal countdown: std_logic_vector(15 downto 0) := (others=>'0');
  signal countdown2: std_logic_vector(15 downto 0) := (others=>'0');
  signal countdown3: std_logic_vector(15 downto 0) := (others=>'0');
  signal state: std_logic_vector(3 downto 0) := "1111";
  signal state2: std_logic_vector(3 downto 0) := "1111";
  signal state3: std_logic_vector(3 downto 0) := "1111";
  signal data: std_logic_vector(7 downto 0) := "11111111";
  signal data2: std_logic_vector(7 downto 0) := "11111111";
  signal go: std_logic :='0';
  type rom_t is array(0 to 3) of std_logic_vector(7 downto 0);
  constant tb_rom : rom_t := (
    "10101010","01010101","11110000","00001111");
  signal addr : std_logic_vector(1 downto 0) := "00";
  component loopback
    port (
      clk: in std_logic;
      rs_tx: out std_logic;
      rs_rx: in std_logic);
    end component;
begin
  
  unit : loopback PORT MAP (
    clk => clk,
    rs_tx => tx,
    rs_rx => rx);
  
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
  rx<=sendbuf(0);

  dataupd: process(clk)
  begin
    if rising_edge(clk) then
      case state3 is
        when "1111"=>
          if countdown3=0 then
            go<='0';
            countdown3<=wtime;
            state3<=state3-1;
          else
            countdown3<=countdown3-1; --1clk毎にデクリメント
          end if;
        when "0000"=>
          if countdown3=0 then--ここで代入
            go<='1';
            data<=tb_rom(conv_integer(addr));
            countdown3<=wtime;
            state3<=state3-1;
            addr<=addr+1;
          else
            countdown3<=countdown3-1; --1clk毎にデクリメント
          end if;
        when others=>
          if countdown3=0 then --wtime*1clktime経てば1
            countdown3<=wtime;
            state3<=state3-1;
          else
            countdown3<=countdown3-1; --1clk毎にデクリメント
          end if;
      end case;
    end if;
  end process;

  recvmsg: process(clk) --受信プロセス
    variable latch : std_logic :='1';
  begin
    if rising_edge(clk) then
      case state2 is
        when "1111"=> --wait state
          --go<='0';
          if tx='0' then --0を受け取ったとき受信開始
            state2<=state2-1;
            countdown2<=wtime;
          end if;
        when "0110"=> --stopbitを受信したらwait stateへ
          if countdown2=0 then
            --go<='1';
            recvbuf<=data2;
            countdown2<=wtime;
            state2<="1111";
          else
            countdown2<=countdown2-1; --1clk毎にデクリメント
          end if;
        when others=>
          if countdown2=0 then --wtime*1clktime経てば1bit受信
            latch := tx; --latchで受ける
            data2<=latch & data2(7 downto 1);
            countdown2<=wtime;
            state2<=state2-1;
          else
            countdown2<=countdown2-1; --1clk毎にデクリメント
          end if;
      end case;
    end if;
  end process;


  clockgen : process
  begin
    clk <= '0';
    wait for 10 ns;
    clk <= '1';
    wait for 10 ns;
  end process;
  
end testbench;
