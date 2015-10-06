LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use std.textio.all;

--

ENTITY FADD_testbench IS
  generic (ARRAY_SIZE : integer := );--ここにARRAYのサイズ-1を入力
END FADD_testbench;

ARCHITECTURE testbench OF FADD_testbench IS 
  -- Component Declaration for the Unit Under Test (UUT)
  COMPONENT fadd
    Port (clk : in std_logic;
      op1,op2  : in  STD_LOGIC_VECTOR (31 downto 0);
      ans :      out STD_LOGIC_VECTOR (31 downto 0));
  END COMPONENT;

  --Inputs
  signal tb_input1  : std_logic_vector(31 downto 0) := (others => '0');
  signal tb_input2  : std_logic_vector(31 downto 0) := (others => '0');
  signal tb_output  : std_logic_vector(31 downto 0) := (others => '0');
  type rom_t is array(0 to ARRAY_SIZE) of std_logic_vector(31 downto 0);
  constant rom : rom_t :=();
  
  --Simulation
  signal simclk : std_logic := '0';
  signal state : std_logic_vector(2 downto 0) := "111";
  signal addr : std_logic_vector(18 downto 0) := "0000000000000000000";
  signal ans1,ans2 : std_logic_vector(31 downto 0) := x"00000000";
  signal miss : std_logic_vector(19 downto 0) := x"00000";



BEGIN
  unit : fadd PORT MAP (
    clk  => simclk,
    op1  => tb_input1,
    op2  => tb_input2,
    ans  => tb_output);

  
  check:process(simclk)
  begin
    if rising_edge(simclk) then
      tb_input1 <= rom(conv_integer(addr));
      tb_input2 <= rom(conv_integer(addr+1));
      ans1 <= rom(conv_integer(addr+2));
      ans2 <= ans1;
      addr<=addr+3;
      if ans2(30 downto 23) = "11111111" and ans2(22 downto 0) /= "00000000000000000000000" then --up to NaN equalism
        if tb_output(30 downto 23) /= "11111111" then
          miss<=miss+1;
        end if;
      elsif ans2(30 downto 0) = "0000000000000000000000000000000" then
	if tb_output(30 downto 0) /= "0000000000000000000000000000000" then
	  miss<=miss+1;
	end if;
      elsif ans2 /= tb_output then
        miss <= miss + 1;
      end if;
    end if;
  end process;
      clockgen : process
  begin
    simclk <= '0';
    wait for 30 ns;
    simclk <= '1';
    wait for 30 ns;
    
  end process;
end testbench;
 
