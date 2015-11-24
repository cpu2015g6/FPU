#include <stdio.h>
#include <unistd.h>
#include "common.h"

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

rob_ring_buffer_type rob_ring_buffer_zero = {{ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO},0,0};
#define ROB_RING_BUFFER_ZERO {{ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO,ROB_ZERO},0,0}

int rob_full(rob_ring_buffer_type rob){//return boolean
  if((rob.rob_array[rob.youngest]).state == ROB_INVALID)
    return 0;
  return 1;
}

typedef int CPU_state_type;//代数的データ型
#define CPU_NORMAL 0
#define CPU_MISPREDICTION 1
#define CPU_LOADING 2

typedef struct{
  decode_result_type decode_result;
  cdb_type cdb;
  register_type registers[32];
  pc_type pc;
  rob_ring_buffer_type rob;
  CPU_state_type state;
  int inst_valid;//std_logic
  int state_sub;//std_logic
}reg_type;

char* deco_rob_state(int o){
  char* s = malloc(sizeof(char) * 5);
  switch(o){
  case 0:
   s = "INVA\0";
   break;
  case 1:
   s = "EXEC\0";
   break;
  case 2:
   s = "DONE\0";
   break;
  case 3:
   s = "RST\0";
   break;
  case 4:
   s = "DUMM\0";
   break;
 }
  return s;
}


void printrob(rob_ring_buffer_type ro){
  fprintf(stderr,"\nrob     :");
  fprintf(stderr,"youngest:  %-3d\n",ro.youngest);
  fprintf(stderr,"oldest  :  %-3d\n\n",ro.oldest); 
  fprintf(stderr,"rob  reg  state  pcnext   data    rob  reg  state  pcnext   data\n");
  int i=0;
  int j=0;
  char* s;
  char* s2;
  for(i=0;i<8;i++){
  j = i+8;
  s = deco_rob_state(ro.rob_array[i].state);
  s2 = deco_rob_state(ro.rob_array[j].state);
  fprintf(stderr,"%-2d   %-2d   %-4s   %-6d   %-7d %-2d   %-2d   %-4s   %-6d   %-7d\n",i,ro.rob_array[i].reg_num,s,ro.rob_array[i].pc_next,ro.rob_array[i].result,
   j,ro.rob_array[j].reg_num,s2,ro.rob_array[j].pc_next,ro.rob_array[j].result);
  }
}

//for sim
void printreg(reg_type r){
  fprintf(stderr,"\n");

  int num=0;
  int i=0;
  int j=0;
  for(;num<32;num++){
   i = num / 4; 
   j = num % 4;
   fprintf(stderr,"reg %2d: %-8d  ",i+8*j,r.registers[i+8*j].data);
   if(j == 3)
    fprintf(stderr,"\n");
  }
  fprintf(stderr,"\n");
}

reg_type reg_zero = {DECODE_RESULT_ZERO,CDB_ZERO,{REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,},0,ROB_RING_BUFFER_ZERO,CPU_NORMAL,0,0};//LOADING ignore
#define REG_ZERO {DECODE_RESULT_ZERO,CDB_ZERO,{REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,REGISTER_ZERO,},0,ROB_RING_BUFFER_ZERO,CPU_NORMAL,0,0}

reg_type r=REG_ZERO;
reg_type r_in=REG_ZERO;

alu_in_type alu_in = ALU_IN_ZERO;
alu_out_type alu_out;
extern alu_out_type alu(int clk, int rst, alu_in_type alu_in);

fpu_in_type fpu_in = FPU_IN_ZERO;
fpu_out_type fpu_out;
extern fpu_out_type fpu(int clk, int rst, fpu_in_type fpu_in);

mem_in_type mem_in = MEM_IN_ZERO;
mem_out_type mem_out;
extern mem_out_type mem(int clk, int rst, mem_in_type mem_in/*mem_op_type rs_in_op,
			int rs_in_has_dummy,
			rs_common_type rs_in_common,cdb_type cdb_in,
			int cdb_next,int sync_rst,int dummy_done,
			sramif_out sramifout,recvif_out_type recvifout,transif_out_type transifout*/);

branch_in_type branch_in = BRANCH_IN_ZERO;
branch_out_type branch_out;
extern branch_out_type branch(int clk,int rst,branch_in_type branch_in);

//program_loader mainから実行

recvif_in_type recvifin_pl;
sramif_in sramifin_pl;
int go_pl=0;//std_logic
int active_pl=0;//std_logic

//procedure branch_predictorは1返り値なので普通に実装

pc_type branch_predictor(decode_result_type decode_result, pc_type pc){
  if(decode_result.opc == J)
    return (pc + decode_result.imm);
  return (pc + 1);
}

//procedure inst_decode は1返り値なので普通に実装

decode_result_type inst_decode(uint32_t inst){//for rev1
  uint32_t opc_rev1=inst / (256*256*256);//inst(31 donwto 24) 
  uint32_t rt_rev1=(inst % (256*256*256)) / (256*256);//(23 downto 16)
  uint32_t ra_rev1=(inst % (256*256)) / 256;//(15 downto 8)
  uint32_t rb_rev1=inst % 256;//(7 downto 0)
  uint32_t imm_rev1=inst % (256*256);//(15 downto 0)

  decode_result_type decode_result= decode_result_zero;
  
  switch(opc_rev1){
  case LIMM:
    decode_result.opc = LIMM;
    decode_result.rt = rt_rev1;
    decode_result.imm = imm_rev1;
    break;
  case CMP:
    decode_result.opc = CMP;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case IN:
    decode_result.opc = IN;
    decode_result.rt = rt_rev1;
    decode_result.need_dummy_rob_entry = 1;
    break;
  case OUT:
    decode_result.opc = OUT;
    decode_result.rt = rt_rev1;
    decode_result.need_dummy_rob_entry = 1;
    break;
  case J:
    decode_result.opc = J;
    decode_result.rt = rt_rev1;
    decode_result.imm = imm_rev1;
    break;
  case JR:
    decode_result.opc = JR;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    break;
  case STW:
    decode_result.opc = STW;
    decode_result.ra = ra_rev1;
    decode_result.rb = rt_rev1;
    decode_result.need_dummy_rob_entry = 1;
    break;
  case LDW:
    decode_result.opc = LDW;
    decode_result.ra = ra_rev1;
    decode_result.rt = rt_rev1;
    break;
  case ADD:
    decode_result.opc = ADD;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case SUB:
    decode_result.opc = SUB;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case AND:
    decode_result.opc = AND;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case OR:
    decode_result.opc = OR;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case XOR:
    decode_result.opc = XOR;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case NOT:
    decode_result.opc = NOT;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case SLL:
    decode_result.opc = SLL;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case SRL:
    decode_result.opc = SRL;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case JREQ:
    decode_result.opc = JREQ;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case JRNEQ:
    decode_result.opc = JRNEQ;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case JRGT:
    decode_result.opc = JRGT;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case JRGTE:
    decode_result.opc = JRGTE;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case JRLT:
    decode_result.opc = JRLT;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case JRLTE:
    decode_result.opc = JRLTE;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case FADD:
    decode_result.opc = FADD;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case FMUL:
    decode_result.opc = FMUL;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case FINV:
    decode_result.opc = FINV;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case FSQRT:
    decode_result.opc = FSQRT;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    break;
  case FCMP:
    decode_result.opc = FCMP;
    decode_result.rt = rt_rev1;
    decode_result.ra = ra_rev1;
    decode_result.rb = rb_rev1;
    break;
  case HALT:
    decode_result.opc = HALT;
    break;
  }
  
  printdecode(decode_result);//for sim

  return decode_result;
}		 

register_type read_reg(reg_num_type reg_num,register_type* regs,rob_type* rob_array){
  register_type reg = regs[reg_num];
  int rob_i = reg.tag.rob_num;
  if(rob_i >= 32){
    fprintf(stderr,"the number of registers is 32\n");
    fprintf(stderr,"register index excess (index %d is refered)\n",rob_i);
    _exit(1);
  }
  if(reg.tag.valid && rob_array[rob_i].state == ROB_DONE){
    reg.data = rob_array[rob_i].result;
    reg.tag = rs_tag_zero;
  }
  if(reg_num == REG_NUM_ZERO)
    return register_zero;
  return reg;
}

//procedure cdb_arbiterはポインタ渡しvoid型として実装

void cdb_arbiter(cdb_type alu_cdb_out,
		 cdb_type fpu_cdb_out,
		 cdb_type mem_cdb_out,
		 cdb_type branch_cdb_out,
		 int* alu_grant,
		 int* fpu_grant,
		 int* mem_grant,
		 int* branch_grant,
		 cdb_type* cdb){
  *alu_grant=0;
  *fpu_grant=0;
  *mem_grant=0;
  *branch_grant=0;
  if(alu_cdb_out.tag.valid){
    *alu_grant=1;
    *cdb=alu_cdb_out;
  }else if(fpu_cdb_out.tag.valid){
    *fpu_grant=1;
    *cdb=fpu_cdb_out;
  }else if(mem_cdb_out.tag.valid){
    *mem_grant=1;
    *cdb=mem_cdb_out;
  }else if(branch_cdb_out.tag.valid){
    *branch_grant=1;
    *cdb=branch_cdb_out;
  }else
    *cdb=cdb_zero;
}

//update_ROBはpointer渡しvoid型で実装
void update_ROB(rob_type* rob_array,cdb_type cdb){
  rob_type new_rob_array[16];
  int rob_i;

  int i=0;
  
  for(i=0;i<16;i++)
    new_rob_array[i] = rob_array[i];
  rob_i = cdb.tag.rob_num;
  if(cdb.tag.valid){
    if((rob_array[rob_i]).reg_num != REG_NUM_ZERO)
      (new_rob_array[rob_i]).result = cdb.data;
    new_rob_array[rob_i].state = ROB_DONE;
    if(cdb.pc_next != (rob_array[rob_i]).pc_next){
      new_rob_array[rob_i].state = ROB_RESET;
      new_rob_array[rob_i].pc_next = cdb.pc_next;
    }
  }
  
  for(i=0;i<16;i++)
    rob_array[i] = new_rob_array[i];
}

cpu_top_out_type cpu_top_out;

cpu_top_out_type cpu_top(int clk, int rst, cpu_top_in_type cpu_top_in){

  //printreg(clk,r,0);
  
  bram_dout = blockram(clk,bram_we,bram_addr,bram_din);//ここに置く
  fprintf(stderr,"pc: %d\n",bram_addr);

  bram_addr_cpu=r_in.pc;
  if(r.state == CPU_LOADING){
    bram_addr = bram_addr_pl;
    cpu_top_out.sramifin = sramifin_pl;
    cpu_top_out.recvifin = recvifin_pl;
  }else{
    bram_addr = bram_addr_cpu;
    cpu_top_out.sramifin = mem_out.sramifin;
    cpu_top_out.recvifin = mem_out.recvifin;
  }
  
  cpu_top_out.transifin = mem_out.transifin;

  int i=0;//loop variant
  
  reg_type v;
  alu_in_type alu_in_v;
  fpu_in_type fpu_in_v;
  mem_in_type mem_in_v;
  branch_in_type branch_in_v;
  alu_rs_type alu_rs_v;
  fpu_rs_type fpu_rs_v;
  mem_rs_type mem_rs_v;
  branch_rs_type branch_rs_v;
  decode_result_type decode_result_v;
  pc_type next_pc;
  unit_type unit;
  int stall;//boolean
  rob_type oldest_rob;
  register_type ra,rb;
  rs_common_type rs_common_3;
  uint32_t zext_imm;
  //int insert_dummy_rob_entry;//boolean
  int go_pl_v;//std_logic

  //関数呼び出しで組み合わせ回路を実現

  alu_out = alu(clk,rst,alu_in);
  fpu_out = fpu(clk,rst,fpu_in);
  mem_out = mem(clk,rst,mem_in
		/*mem_in.rs_in.op,
		mem_in.rs_in.has_dummy,
		mem_in.rs_in.common,
		mem_in.cdb_in,
		mem_in.cdb_next,
		mem_in.rst,
		mem_in.dummy_done,
		mem_in.sramifout,
		mem_in.recvifout,
		mem_in.transifout*/);
  branch_out = branch(clk,rst,branch_in);
  
  v=r;
  alu_in_v = alu_in_zero;
  fpu_in_v = fpu_in_zero;
  mem_in_v = mem_in_zero;
  branch_in_v = branch_in_zero;
  go_pl_v=0;
  
  switch(r.state){
  case CPU_NORMAL:
    if(r.inst_valid == 0){
      v.pc=r.pc;
      v.inst_valid = 1;
    }else{
      update_ROB(v.rob.rob_array, r.cdb);
    
      decode_result_v = inst_decode(bram_dout);
    
      if(decode_result_v.need_dummy_rob_entry == 1 && r.decode_result.need_dummy_rob_entry == 1)
	decode_result_v.need_dummy_rob_entry = 0;
    
      next_pc=branch_predictor(decode_result_v,r.pc);
    
      if(decode_result_v.need_dummy_rob_entry)
	next_pc = r.pc;
    
      decode_result_v.pc = r.pc;
      decode_result_v.pc_predicted = next_pc;
      ra = read_reg(r.decode_result.ra, r.registers, v.rob.rob_array);
      rb = read_reg(r.decode_result.rb, r.registers, v.rob.rob_array);
      alu_rs_v = alu_rs_zero;
      fpu_rs_v = fpu_rs_zero;
      mem_rs_v = mem_rs_zero;
      branch_rs_v = branch_rs_zero;
      zext_imm = r.decode_result.imm;
      rs_common_3.ra=ra;
      rs_common_3.rb=rb;
      rs_common_3.state=RS_WAITING;
      rs_common_3.result=0;
      rs_common_3.rob_num=r.rob.youngest;
      rs_common_3.pc=r.decode_result.pc;
      rs_common_3.pc_next=0;
      unit=NULL_UNIT;
      
      switch(r.decode_result.opc){
      case LIMM:
	unit=ALU_UNIT;
	alu_rs_v.op=LIMM;
	alu_rs_v.common.ra.data = zext_imm;
	alu_rs_v.common.ra.tag = rs_tag_zero;
	alu_rs_v.common.rb = register_zero;
	alu_rs_v.common.state = RS_WAITING;
	alu_rs_v.common.result = 0;
	alu_rs_v.common.rob_num = r.rob.youngest;
	alu_rs_v.common.pc = r.decode_result.pc;
	alu_rs_v.common.pc_next = 0;
	break;
      case CMP:
	unit = ALU_UNIT;
	alu_rs_v.op = CMP;
	alu_rs_v.common = rs_common_3;
	break;
      case ADD:
	unit = ALU_UNIT;
	alu_rs_v.op = ADD;
	alu_rs_v.common = rs_common_3;
	break;
      case SUB:
	unit = ALU_UNIT;
	alu_rs_v.op = SUB;
	alu_rs_v.common = rs_common_3;
	break;
      case AND:
	unit = ALU_UNIT;
	alu_rs_v.op = AND;
	alu_rs_v.common = rs_common_3;
	break;
      case OR:
	unit = ALU_UNIT;
	alu_rs_v.op = OR;
	alu_rs_v.common = rs_common_3;
	break;
      case XOR:
	unit = ALU_UNIT;
	alu_rs_v.op = XOR;
	alu_rs_v.common = rs_common_3;
	break;
      case NOT:
	unit = ALU_UNIT;
	alu_rs_v.op = NOT;
	alu_rs_v.common = rs_common_3;
	break;
      case SLL:
	unit = ALU_UNIT;
	alu_rs_v.op = SLL;
	alu_rs_v.common = rs_common_3;
	break;
      case SRL:
	unit = ALU_UNIT;
	alu_rs_v.op = SRL;
	alu_rs_v.common = rs_common_3;
	break;
      case HALT://simulator only
	unit = ALU_UNIT;
	alu_rs_v.op = HALT;
	alu_rs_v.common = rs_common_3;
	break;
      case FADD:
	unit = FPU_UNIT;
	fpu_rs_v.op = FADD;
	fpu_rs_v.common = rs_common_3;
	break;
      case FMUL:
	unit = FPU_UNIT;
	fpu_rs_v.op = FMUL;
	fpu_rs_v.common = rs_common_3;
	break;
      case FINV:
	unit = FPU_UNIT;
	fpu_rs_v.op = FINV;
	fpu_rs_v.common = rs_common_3;
	break;
      case FSQRT:
	unit = FPU_UNIT;
	fpu_rs_v.op = FSQRT;
	fpu_rs_v.common = rs_common_3;
	break;
      case FCMP:
	unit = FPU_UNIT;
	fpu_rs_v.op = FCMP;
	fpu_rs_v.common = rs_common_3;
	break;
      case J:
	unit = BRANCH_UNIT;
	branch_rs_v.op = J;
	branch_rs_v.common.ra.data = zext_imm;
	branch_rs_v.common.ra.tag = rs_tag_zero;
	branch_rs_v.common.rb = register_zero;
	branch_rs_v.common.state = RS_WAITING;
	branch_rs_v.common.result = 0;
	branch_rs_v.common.rob_num = r.rob.youngest;
	branch_rs_v.common.pc = r.decode_result.pc;
	branch_rs_v.common.pc_next = 0;
	break;
      case JR:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JR;
	branch_rs_v.common.ra = ra;
	branch_rs_v.common.rb = register_zero;
	branch_rs_v.common.state = RS_WAITING;
	branch_rs_v.common.result = 0;
	branch_rs_v.common.rob_num = r.rob.youngest;
	branch_rs_v.common.pc = r.decode_result.pc;
	branch_rs_v.common.pc_next = 0;
	break;
      case JREQ:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JREQ;
	branch_rs_v.common = rs_common_3;
	break;
      case JRNEQ:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JRNEQ;
	branch_rs_v.common = rs_common_3;
	break;
      case JRGT:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JRGT;
	branch_rs_v.common = rs_common_3;
	break;
      case JRGTE:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JRGTE;
	branch_rs_v.common = rs_common_3;
	break;
      case JRLT:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JRLT;
	branch_rs_v.common = rs_common_3;
	break;
      case JRLTE:
	unit = BRANCH_UNIT;
	branch_rs_v.op = JRLTE;
	branch_rs_v.common = rs_common_3;
	break;
      case STW:
	unit = MEM_UNIT;
	mem_rs_v.op = STORE;
	mem_rs_v.has_dummy = 1;
	mem_rs_v.common.ra = ra;
	mem_rs_v.common.rb = rb;
	mem_rs_v.common.state = RS_WAITING;
	mem_rs_v.common.result = 0;
	mem_rs_v.common.rob_num = r.rob.youngest;
	mem_rs_v.common.pc = r.decode_result.pc;
	mem_rs_v.common.pc_next = 0;
	break;
      case LDW:
	unit = MEM_UNIT;
	mem_rs_v.op = LOAD;
	mem_rs_v.common.ra = ra;
	mem_rs_v.common.rb = register_zero;
	mem_rs_v.common.state = RS_WAITING;
	mem_rs_v.common.result = 0;
	mem_rs_v.common.rob_num = r.rob.youngest;
	mem_rs_v.common.pc = r.decode_result.pc;
	mem_rs_v.common.pc_next = 0;
	break;
      case NOP:
	unit = NULL_UNIT;
	break;
      case IN:
	unit = MEM_UNIT;
	mem_rs_v.op = IN;
	mem_rs_v.has_dummy = 1;
	mem_rs_v.common.ra = register_zero;
	mem_rs_v.common.rb = register_zero;
	mem_rs_v.common.state = RS_WAITING;
	mem_rs_v.common.result = 0;
	mem_rs_v.common.rob_num = r.rob.youngest;
	mem_rs_v.common.pc = r.decode_result.pc;
	mem_rs_v.common.pc_next = 0;
	break;
      case OUT:
	unit = MEM_UNIT;
	mem_rs_v.op = OUT;
	mem_rs_v.has_dummy = 1;
	mem_rs_v.common.ra = ra;
	mem_rs_v.common.rb = register_zero;
	mem_rs_v.common.state = RS_WAITING;
	mem_rs_v.common.result = 0;
	mem_rs_v.common.rob_num = r.rob.youngest;
	mem_rs_v.common.pc = r.decode_result.pc;
	mem_rs_v.common.pc_next = 0;
	break;
      }//decode

      if(r.decode_result.need_dummy_rob_entry == 1){
	unit = NULL_UNIT;
	alu_rs_v = alu_rs_zero;
	fpu_rs_v = fpu_rs_zero;
	mem_rs_v = mem_rs_zero;
	branch_rs_v = branch_rs_zero;
      }

      stall = rob_full(r.rob);
    
      if(stall){
	fprintf(stderr,"<<<<<<<<<<<<<<<<<<<<rob_full>>>>>>>>>>>>>>>>>>>>\n");
      }
    
      switch(unit){
      case ALU_UNIT:
	stall = (stall || alu_out.rs_full);
	break;
      case FPU_UNIT:
	stall = (stall || fpu_out.rs_full);
	break;
      case MEM_UNIT:
	stall = (stall || mem_out.rs_full);
	break;
      case BRANCH_UNIT:
	stall = (stall || branch_out.rs_full);
	break;
      case NULL_UNIT:
	break;
      }

      alu_in_v.cdb_in = r.cdb;
      fpu_in_v.cdb_in = r.cdb;
      mem_in_v.cdb_in = r.cdb;
      branch_in_v.cdb_in = r.cdb;

      //fprintf(stderr,"<<<dbg>>>\n\n");
      //printrob(v.rob);
      //fprintf(stderr,"<<<dbg>>>\n\n");

      if(!stall){
	v.decode_result = decode_result_v;
	v.pc = next_pc;
	alu_in_v.rs_in = alu_rs_v;
	fpu_in_v.rs_in = fpu_rs_v;
	mem_in_v.rs_in = mem_rs_v;
	branch_in_v.rs_in = branch_rs_v;
	if(r.decode_result.opc != NOP){
	  if(r.decode_result.need_dummy_rob_entry == 1){
	    v.rob.rob_array[r.rob.youngest].state = ROB_DUMMY;
	    v.rob.rob_array[r.rob.youngest].pc_next = r.decode_result.pc_predicted;
	    v.rob.rob_array[r.rob.youngest].result = 0;
	    v.rob.rob_array[r.rob.youngest].reg_num = REG_NUM_ZERO;
	  }else{
	    v.rob.rob_array[r.rob.youngest].state = ROB_EXECUTING;
	    v.rob.rob_array[r.rob.youngest].pc_next = r.decode_result.pc_predicted;
	    v.rob.rob_array[r.rob.youngest].result = 0;
	    v.rob.rob_array[r.rob.youngest].reg_num = r.decode_result.rt;
	  }

	  if(r.decode_result.rt != REG_NUM_ZERO){
	    v.registers[r.decode_result.rt].tag.valid = 1;
	    v.registers[r.decode_result.rt].tag.rob_num = r.rob.youngest;
	  }
	  v.rob.youngest = r.rob.youngest + 1;
	}
      }
      
      if(stall){
	fprintf(stderr,"stall\n");
      }

      cdb_arbiter(alu_out.cdb_out,fpu_out.cdb_out,mem_out.cdb_out,branch_out.cdb_out,&(alu_in_v.cdb_next),&(fpu_in_v.cdb_next),&(mem_in_v.cdb_next),&(branch_in_v.cdb_next),&(v.cdb));
		  
      //commit ROB
      oldest_rob = v.rob.rob_array[v.rob.oldest];
      if(oldest_rob.state == ROB_DUMMY){
	mem_in_v.dummy_done = 1;
	v.rob.rob_array[v.rob.oldest] = rob_zero;
	v.rob.oldest = v.rob.oldest + 1;
      }else if(oldest_rob.state == ROB_DONE){
	if(oldest_rob.reg_num != REG_NUM_ZERO){
	  //assert
	  v.registers[oldest_rob.reg_num].data = oldest_rob.result;
	  if(v.registers[oldest_rob.reg_num].tag.rob_num == v.rob.oldest){
	    v.registers[oldest_rob.reg_num].tag = rs_tag_zero;
	  }
	}
	v.rob.rob_array[v.rob.oldest] = rob_zero;
	v.rob.oldest = v.rob.oldest + 1;
      }else if(oldest_rob.state == ROB_RESET){
	fprintf(stderr,"rob reset\n");
	if(oldest_rob.reg_num != REG_NUM_ZERO){
	  //assert
	  v.registers[oldest_rob.reg_num].data = oldest_rob.result;
	}
	for(i=0;i<32;i++){
	  v.registers[i].tag = rs_tag_zero;
	}
	v.decode_result = decode_result_zero;
	v.cdb = cdb_zero;
	//v.registers = v.registers;
	v.pc = oldest_rob.pc_next;
	v.rob = rob_ring_buffer_zero;
	v.state = CPU_NORMAL;
	v.inst_valid = '0';
	v.state_sub = '0';
	// reset other modules
	alu_in_v.rst = '1';
	fpu_in_v.rst = '1';
	mem_in_v.rst = '1';
	branch_in_v.rst = '1';
      }
    }
  case CPU_LOADING:
    switch(r.state_sub){
    case 0:
      go_pl_v = 1;
      v.state_sub = 1;
      break;
    case 1:
      if(active_pl == 0){
	v.state = CPU_NORMAL;
      }
      break;
    }
  }
      
  go_pl = go_pl_v;
  alu_in = alu_in_v;
  fpu_in = fpu_in_v;
  mem_in_v.sramifout = cpu_top_in.sramifout;
  mem_in_v.recvifout = cpu_top_in.recvifout;
  mem_in_v.transifout = cpu_top_in.transifout;
  mem_in = mem_in_v;
  branch_in = branch_in_v;
  r_in = v;

  fprintf(stderr,"---------------------------------------------%d times clk rising----------------------------------------------\n\n",clk+1);

  fprintf(stderr,"sram[0]: %d\n",sram[0]);
  fprintf(stderr,"sram[1]: %d\n",sram[1]);
  fprintf(stderr,"sram[2]: %d\n",sram[2]);
  fprintf(stderr,"sram[3]: %d\n",sram[3]);
  fprintf(stderr,"sram[4]: %d\n",sram[4]);
  fprintf(stderr,"sram[5]: %d\n",sram[5]);
  fprintf(stderr,"sram[6]: %d\n",sram[6]);

  if(rst)//clk rising
    r=reg_zero;
  else
    r=r_in;

  printrob(r.rob);

  printreg(r);

  return cpu_top_out;
}
