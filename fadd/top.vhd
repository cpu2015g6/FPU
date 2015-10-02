library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.STD_LOGIC_ARITH.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity top is
  port(MCLK1: in std_logic;
       RS_TX: out std_logic);
  end top;

architecture VHDL of top is

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
  type rom_t is array(0 to 8) of std_logic_vector(31 downto 0);
  constant rom: rom_t := ("00111111100000000000000000000000",
"00111111100000000000000000000000",
"01000000000000000000000000000000",
"00111111100000000000000000000000",
"00111111100000000000000000000000",
"01000000000000000000000000000000",
"10111111100000000000000000000000",
"10111111100000000000000000000000",
"11000000000000000000000000000000");

  signal op1,op2,ans1,ans2,ans: std_logic_vector(31 downto 0);
  signal addr: std_logic_vector(9 downto 0) := (others=>'0');
  signal miss: std_logic_vector(16 downto 0) := (others=>'0');
  signal rom_o: std_logic_vector(7 downto 0) := (others=>'1');
  signal uart_go: std_logic;
  signal uart_busy: std_logic := '0';
  signal co: std_logic_vector(2 downto 0) := "000";
  signal a: std_logic_vector(31 downto 0) := (others=>'0');

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
   if rising_edge(clk) then--3clk後に返答
     if co = "000" then
       addr<=addr+3;
       op1<=rom(conv_integer(addr));
       op2<=rom(conv_integer(addr+1));
       ans1<=rom(conv_integer(addr+2));
       ans2<=ans1;
       if ans2(30 downto 23) = "11111111" and ans2(22 downto 0) /= "00000000000000000000000" then --up to NaN equalism
        if ans(30 downto 23) /= "11111111" then
          miss<=miss+1;
        end if;
       elsif ans2(30 downto 0) = "0000000000000000000000000000000" then
	if ans(30 downto 0) /= "0000000000000000000000000000000" then
	  miss<=miss+1;
	end if;
       elsif ans2 /= ans then
         miss<=miss+1;
       end if;
       if addr = "0000000011" then
         co<=co+1;
         a<=ans2;
       end if;
     elsif co = "001" and uart_go='1' then
       rom_o<=x"62";
       co<=co+1;
     elsif co = "010" and uart_go='1' then
       rom_o<=a(31 downto 24);
       co<=co+1;
     elsif co = "011" and uart_go='1' then
       rom_o<=a(23 downto 16);
       co<=co+1;
     elsif co = "100" and uart_go='1' then
       rom_o<=a(15 downto 8);
       co<=co+1;
     elsif co = "101" and uart_go='1' then
       rom_o<=a(7 downto 0);
       co<="001";
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
