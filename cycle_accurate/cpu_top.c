#include <stdio.h>
#include "common.h"

uint32_t ram[1048576];//4byte*1M memory

uint32_t blockram(int clk, int we, uint32_t addr, uint32_t din, uint32_t dout){
  if(we)
    ram[addr] = din;
  if(addr >= 1048576){
    fprintf(stderr,"sram index broken\n");
    _exit(1);
  }
  return ram[addr];
}

int bram_we = 0;//std_logic
uint32_t bram_addr,bram_addr_cpu,bram_addr_pl;//vector pc_width-1 downto 0
uint32_t bram_din = 0;//vec 31 downto 0
uint32_t bram_dout = 0;//vec 31 downto 0

//mallocめんどいのでrob_array_typeは後で配列宣言

typedef struct{
  rob_type rob_array[16];//rob_array_type
  uint32_t youngest;//vec 3 downto 0
  uint32_t oldest;//vec 3 downto 0
}rob_ring_buffer_type;

rob_ring_buffer_type rob_ring_buffer_zero = {{rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero,rob_zero},0,0};

int rob_full(rob_ring_buffer_type rob){//return boolean
  if((rob.rob_array[rob.youngest]).state == ROB_INVALID)
    return 0;
  return 1;
}

typedef CPU_state_type int;//代数的データ型
#define CPU_NORMAL 0
#define CPU_MISPREDICTION 1
#define CPU_LOADING 2

typedef struct{
  decode_result_type decode_result;
  cdb_type cdb;
  register_array_type registers;
  pc_type pc;
  rob_ring_buffer_type rob;
  CPU_state_type state;
  int inst_valid;//std_logic
  int state_sub;//std_logic
}reg_type;

reg_type reg_zero = {decode_result_zero,cdb_zero,register_array_zero,0,rob_ring_buffer_zero,CPU_LOADING,0,0};

reg_type r=reg_zero;
reg_type r_in=reg_zero;

alu_in_type alu_in = alu_in_zero;
alu_out_type alu_out;
extern alu_out_type alu(int clk, int rst, alu_in_type alu_in);

fpu_in_type fpu_in = fpu_in_zero;
fpu_out_type fpu_out;
extern fpu_out_type fpu(int clk, int rst, fpu_in_type fpu_in);

mem_in_type mem_in = mem_in_zero;
mem_out_type mem_out;
extern mem_out_type mem(int clk, int rst, mem_op_type rs_in_op,
			int rs_in_has_dummy/*std_logic*/,
			rs_common_type rs_in_common,cdb_type cdb_in,
			int cdb_next,int sync_rst,int dummy_done/*3つのstd_logic*/,
			sramif_out sramifout,recvif_out_type recvifout,transif_out_type transifout);

branch_in_type branch_in = branch_in_zero;
branch_out_type branch_out;
extern branch_out_type branch(int clk,int rst,branch_pack_in_type branch_in,branch_out_type branch_out);

//program loader 未実装

//procedure branch_predictorは1返り値なので普通に実装

pc_type branch_predictor(decode_result_type decode_result, pc_type pc){
  if(decode_result.opc == J)
    return (pc + decode_result.imm);
  return (pc + 1);
}

//procedure inst_decode は1返り値なので普通に実装

decode_result_type inst_decode(uint32_t inst/*vec 31 downto 0*/){//for rev1
  uint32_t opc_rev1=inst / (256*256*256);//inst(31 donwto 24) 
  uint32_t rt_rev1=(inst % (256*256*256)) / (256*256);//(23 downto 16)
  uint32_t ra_rev1=(inst % (256*256)) / 256;//(15 downto 8)
  uint32_t rb_rev1=inst % 256;//(7 downto 0)
  uint32_t imm_rev1=inst % (256*256);//(15 downto 0)

  //decode_result_type returnvalue = decode_result_zero;
  
  return simulation_only_decode(opc_rev1,rt_rev1,ra_rev1,rb_rev1,imm_rev1);
}		 

register_type read_reg(reg_num_type reg_num,register_array_type regs,rob_array_type rob_array){
  register_type reg = regs[reg_num];
  int rob_i = reg.tag.rob_num;
  if(reg.tag.valid && rob_array[rob_i].state == ROB_DONE){
    reg.data = rob_array[rob_i].result;
    tag = rs_tag_zero;
  }
  if(reg_num == reg_num_zero)
    return register_zero;
  return reg;
}

//procedure cdb_arbiterはポインタ渡しvoid型として実装

void cdb_arbiter(cdb_type alu_cdb_out,
		 cdb_type fpu_cdb_out,
		 cdb_type mem_cdb_out,
		 cdb_type branch_cdb_out,
		 cdb_type* alu_grant,
		 cdb_type* fpu_grant,
		 cdb_type* mem_grant,
		 cdb_type* branch_grant,
		 cdb_type* cdb,){
  alu_grant=0;
  fpu_grant=0;
  mem_grant=0;
  branch_grant=0;
  if(alu_cdb_out.tag.valid){
    alu_grant=1;
    cdb=alu_cdb_out;
  }else if(fpu_cdb_out.tag.valid){
    fpu_grant=1;
    cdb=fpu_cdb_out;
  }else if(mem_cdb_out.tag.valid){
    mem_grant=1;
    cdb=mem_cdb_out;
  }else if(branch_cdb_out.tag.valid){
    branch_grant=1;
    cdb=branch_cdb_out;
  }else
    cdb=cdb.zero;
}

rob_array_type update_ROB(rob_array_type rob_array,cdb_type cdb){
  rob_array_type new_rob_array;
  int rob_i;
  new_rob_array = rob_array;
  rob_i = cdb.tag.rob_num;
  if(cdb.tag.valid){
    if((rob_array[rob_i]).reg_num != reg_num_zero)
      (new_rob_array[rob_i]).result = cdb.data;
    new_rob_array[rob_i].state = ROB_DONE;
    if(cdb.pc_next != (rob_array[rob_i]).pc_next){
      new_rob_array[rob_i].state = ROB_RESET;
      new_rob_array[rob_i].pc_next = cdb.pc_next;
    }
  }
  return new_rob_array;
}

int cpu_top(int clk, int rst, cpu_top_in_type cpu_top_in){
  
  bram_addr=r_in.pc;
  if(r.state == CPU_LOADING)
  cpu_top_out.sramifin = mem_out.sramifin;
  cpu_top_out.recvifin = mem_out.recvifin;
  cpu_top_out.transifin = mem_out.transifin;
  
  
