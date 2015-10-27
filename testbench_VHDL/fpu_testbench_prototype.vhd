LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use std.textio.all;

-- test FPU which isn't synchronized to clk

ENTITY FPU_testbench IS
  generic (ARRAY_SIZE : integer := );--ここにARRAYのサイズ-1を入力
END FPU_testbench;

ARCHITECTURE testbench OF FPU_testbench IS 
  -- Component Declaration for the Unit Under Test (UUT)
  COMPONENT f  --any component of FPU
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
  signal pipesize: std_logic_vector(3 downto 0);
  signal state : std_logic_vector(2 downto 0) := "111";
  signal addr : std_logic_vector(18 downto 0) := "0000000000000000000";
  signal low,high : std_logic_vector(31 downto 0) := x"00000000";
  signal miss : std_logic_vector(19 downto 0) := x"00000";


BEGIN
  unit : f PORT MAP (-- any port of FPU
    clk  => simclk,
    op1  => tb_input1,
    op2  => tb_input2,
    ans  => tb_output);

  
  check:process(simclk)
  begin
    if rising_edge(simclk) then
      tb_input1 <= rom(conv_integer(addr));
      tb_input2 <= rom(conv_integer(addr+1));
      low <= rom(conv_integer(addr+2));
      high <= rom(conv_integer(addr+3));
      addr<=addr+4;
      low2<=low;
      high2<=high;
      if not((low2 <= tb_output) and (high2 >= tb_output)) then
        miss <= miss + 1;
      end if;
    end if;
  end process;
      clockgen : process
  begin
    simclk <= '0';
    wait for 5 ns;
    simclk <= '1';
    wait for 5 ns;
    
  end process;
end testbench;
 

