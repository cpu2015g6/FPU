#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
#define RS_WAITING 1
#define NOP 0
#define REG_NUM_ZERO 15
#define RS_TAG_ZERO {0,0}
#define RS_INVALID 0
#define REGISTER_ZERO {0,RS_TAG_ZERO}
#define ALU_NOP_OP 0
#define RS_COMMON_ZERO {RS_INVALID,REGISTER_ZERO,REGISTER_ZERO,0,0,0,0}
#define ALU_RS_ZERO {ALU_NOP_OP,RS_COMMON_ZERO}
#define CDB_ZERO {0,RS_TAG_ZERO,0}
#define FPU_NOP_OP 0
#define FPU_RS_ZERO {FPU_NOP_OP,0,RS_COMMON_ZERO}
#define MEM_NOP_OP 0
#define MEM_RS_ZERO {MEM_NOP_OP,0,RS_COMMON_ZERO}
#define SRAMIF_OUT_ZERO {0,0}
#define RECVIF_OUT_ZERO {0,0,0}
#define TRANSIF_OUT_ZERO {0}
#define SRAMIF_IN_ZERO {0,0,0}
#define RECVIF_IN_ZERO {0}
#define TRANSIF_IN_ZERO {0,0}
#define BRANCH_NOP_OP 0
#define BRANCH_RS_ZERO {BRANCH_NOP_OP,RS_COMMON_ZERO}
*/

typedef uint32_t rs_num_type;//vector 1 downto 0
typedef uint32_t pc_type;//vec 13 downto 0
typedef uint32_t rob_num_type;// vec 3 downto 0
typedef uint32_t reg_num_type;// vec 4 downto 0
typedef int opc_type;//代数的データ型
typedef int unit_type;//代数的データ型

typedef struct{
  int valid;//std_logic
  rob_num_type rob_num;
}rs_tag_type;

typedef struct{
  opc_type opc;
  reg_num_type rt;
  reg_num_type ra;
  reg_num_type rb;
  uint32_t imm;//vec 15 downto 0
  pc_type pc;
  pc_type pc_predicted;
  int need_dummy_rob_entry;//std_logic
}decode_result_type;

typedef struct{
  uint32_t data;
  rs_tag_type tag;
}register_type;

typedef struct{
  uint32_t data;//31 downto 0
  rs_tag_type tag;
  pc_type pc_next;
}cdb_type;

typedef int rs_state_type;

typedef struct{
  rs_state_type state;
  register_type ra;
  register_type rb;
  uint32_t result;//31 downto 0
  rob_num_type rob_num;
  pc_type pc;
  pc_type pc_next;
}rs_common_type;

typedef int rob_state_type;//Invalid->0

typedef struct{
  rob_state_type state;
  pc_type pc_next;
  uint32_t result;//31 downto 0
  reg_num_type reg_num;
}rob_type;

typedef int sramif_op;//NOP->0

typedef struct{
  sramif_op op;
  uint32_t addr;//19 downto 0
  uint32_t wd;//31 downto 0
}sramif_in;

typedef struct{
  int data_enable;//std_logic
  uint32_t rd;
}sramif_out;

typedef struct{
  int rd_en;//std_logic
}recvif_in_type;

typedef struct{
  uint32_t dout;//7 downto 0
  int full;//std_logic
  int empty;//std_logic
}recvif_out_type;

typedef struct{
  int wr_en;//std_logic
  uint32_t din;//7 downto 0
}transif_in_type;

typedef struct{
  int full;//std_logic
}transif_out_type;

typedef struct{
  sramif_out sramifout;
  recvif_out_type recvifout;
  transif_out_type transifout;
}cpu_top_in_type;

typedef struct{
  sramif_in sramifin;
  recvif_in_type recvifin;
  transif_in_type transifin;
}cpu_top_out_type;

typedef int alu_op_type;

typedef struct{
  alu_op_type op;
  rs_common_type common;
}alu_rs_type;

typedef struct{
  alu_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}alu_in_type;

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}alu_out_type;

typedef int fpu_op_type;

typedef struct{
  fpu_op_type op;
  int countdown;//vec 2 downto 0
  rs_common_type common;
}fpu_rs_type;

typedef struct{
  fpu_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}fpu_in_type;

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}fpu_out_type;

typedef int mem_op_type;

typedef struct{
  mem_op_type op;
  int has_dummy;//std_logic
  rs_common_type common;
}mem_rs_type;

typedef struct{
  mem_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
  int dummy_done;//std_logic
  sramif_out sramifout;
  recvif_out_type recvifout;
  transif_out_type transifout;
}mem_in_type;

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
  sramif_in sramifin;
  recvif_in_type recvifin;
  transif_in_type transifin;
}mem_out_type;

typedef int branch_op_type;

typedef struct{
  branch_op_type op;
  rs_common_type common;
}branch_rs_type;

typedef struct{
  branch_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}branch_in_type;

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}branch_out_type;

uint32_t bram[262144];//1MB memory
/*
rs_tag_type rs_tag_zero = {0,0};
decode_result_type decode_result_zero={NOP,REG_NUM_ZERO,REG_NUM_ZERO,REG_NUM_ZERO,0,0,0,0};
register_type register_zero ={0,RS_TAG_ZERO};
cdb_type cdb_zero = {0,RS_TAG_ZERO,0};
rs_common_type rs_common_zero = {RS_INVALID,REGISTER_ZERO,REGISTER_ZERO,0,0,0,0};
rob_type rob_zero={0,0,0,0};
sramif_in sramif_in_zero={0,0,0};
sramif_out sramif_out_zero={0,0};
recvif_in_type recvif_in_zero={0};
recvif_out_type recvif_out_zero={0,0,0};
transif_in_type transif_in_zero={0,0};
transif_in_type transif_out_zero={0};
alu_rs_type alu_rs_zero={ALU_NOP_OP,RS_COMMON_ZERO};
alu_in_type alu_in_zero = {ALU_RS_ZERO,CDB_ZERO,0,0};
alu_out_type alu_out_zero = {0,CDB_ZERO};
fpu_rs_type fpu_rs_zero={FPU_NOP_OP,0,RS_COMMON_ZERO};
fpu_in_type fpu_in_zero = {FPU_RS_ZERO,CDB_ZERO,0,0};
fpu_out_type fpu_out_zero = {0,CDB_ZERO};
mem_rs_type mem_rs_zero={MEM_NOP_OP,0,RS_COMMON_ZERO};
mem_in_type mem_in_zero = {MEM_RS_ZERO,CDB_ZERO,0,0,0,SRAMIF_OUT_ZERO,RECVIF_OUT_ZERO,TRANSIF_OUT_ZERO};
mem_out_type mem_out_zero = {0,CDB_ZERO,SRAMIF_IN_ZERO,RECVIF_IN_ZERO,TRANSIF_IN_ZERO};
branch_rs_type branch_rs_zero={BRANCH_NOP_OP,RS_COMMON_ZERO};
branch_in_type branch_in_zero = {BRANCH_RS_ZERO,CDB_ZERO,0,0};
branch_out_type branch_out_zero = {0,CDB_ZERO};
*/

rs_tag_type rs_tag_zero = {0,0};
decode_result_type decode_result_zero={0,15,15,15,0,0,0,0};
register_type register_zero ={0,{0,0}};
cdb_type cdb_zero = {0,{0,0},0};
rs_common_type rs_common_zero = {0,{0,{0,0}},{0,{0,0}},0,0,0,0};
rob_type rob_zero={0,0,0,0};
sramif_in sramif_in_zero={0,0,0};
sramif_out sramif_out_zero={0,0};
recvif_in_type recvif_in_zero={0};
recvif_out_type recvif_out_zero={0,0,0};
transif_in_type transif_in_zero={0,0};
transif_in_type transif_out_zero={0};
alu_rs_type alu_rs_zero={0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}};
alu_in_type alu_in_zero = {{0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}},{0,{0,0},0},0,0};
alu_out_type alu_out_zero = {0,{0,{0,0},0}};
fpu_rs_type fpu_rs_zero={0,0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}};
fpu_in_type fpu_in_zero = {{0,0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}},{0,{0,0},0},0,0};
fpu_out_type fpu_out_zero = {0,{0,{0,0},0}};
mem_rs_type mem_rs_zero={0,0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}};
mem_in_type mem_in_zero = {{0,0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}},{0,{0,0},0},0,0,0,{0,0},{0,0,0},{0}};
mem_out_type mem_out_zero = {0,{0,{0,0},0},{0,0,0},{0},{0,0}};
branch_rs_type branch_rs_zero={0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}};
branch_in_type branch_in_zero = {{0,{0,{0,{0,0}},{0,{0,0}},0,0,0,0}},{0,{0,0},0},0,0};
branch_out_type branch_out_zero = {0,{0,{0,0},0}};

int rs_common_ready(rs_common_type r){//booleanの代わりにint
  if(r.state == 1 && r.ra.tag.valid == 0 && r.rb.tag.valid == 0)
    return 1;
  return 0;
}

int my_equal(rs_tag_type a, rs_tag_type b){
  if(a.valid == b.valid){
    if(a.rob_num == b.rob_num)
      return 1;
  }
  return 0;
}

register_type register_update(register_type reg, cdb_type cdb){
  register_type v;
  if(cdb.tag.valid == 1 && my_equal(cdb.tag,reg.tag)){
    v.tag.valid = 0;
    v.tag.rob_num = 0;
    v.data = cdb.data;
  }else
    v=reg;
  return v;
}

cdb_type make_cdb_out(rs_common_type rs_common){
  cdb_type c;
  c.tag.valid=1;
  c.tag.rob_num=rs_common.rob_num;
  c.data=rs_common.result;
  c.pc_next=rs_common.pc_next;
  return c;
}

uint32_t blockram(int clk, int we, uint32_t addr, uint32_t din){
  if(we)
    bram[addr] = din;
  if(addr >= 262144){
    fprintf(stderr,"bram index broken\n");
    _exit(1);
  }
  return bram[addr];
}

void program_loader(char* binary){
  FILE* fp = fopen(binary,"r");
  uint32_t* buf = malloc(4);
  uint32_t* buf2 = bram;
  uint32_t a1,a2,a3,a4;
  //int i=0;
  for(;fread(buf,4,1,fp);buf2++){ 
      a4 = buf[0] % 256;
      a3 = (buf[0] / 256) % 256;
      a2 = (buf[0] / (256 * 256)) % 256;
      a1 = buf[0] / (256 * 256 * 256);
      //if(i < 10)
        //fprintf(stdout,"%d clk %d %d %d %d \n",i,a1,a2,a3,a4);
      //i++;
      buf2[0] = a4 * 0x1000000 + a3 * 0x10000 + a2 * 0x100 + a1 * 1;
  }
  fclose(fp);
}

char* deco_rs_state(int o){
  char* s = malloc(sizeof(char)*5);
  switch(o){
  case 0:
   s = "INVA\0";
   break;
  case 1:
   s = "WAIT\0";
   break;
  case 2:
   s = "EXEC\0";
   break;
  case 3:
   s = "DONE\0";
   break;
  case 4:
   s = "RESV\0";
   break;
  }
 return s;
}

char* deco(int o){
  char* s = malloc(sizeof(char)*6);
  switch(o){
  case 0xD0:
    s = "LIMM\0";
    break;
  case 0xD1:
    s = "CMP\0";
    break;
  case 0xD2:
    s = "IN\0";
    break;
  case 0xD3:
    s = "OUT\0";
    break;
  case 0xD4:
    s = "J\0";
    break;
  case 0xD5:
    s = "JR\0";
    break;
  case 0xF0:
    s = "JREQ\0";
    break;
  case 0xF1:
    s = "JRNEQ\0";
    break;
  case 0xF2:
    s = "JRGT\0";
    break;
  case 0xF3:
    s = "JRGTE\0";
    break;
  case 0xF4:
    s = "JRLT\0";
    break;
  case 0xF5:
    s = "JRLTE\0";
    break;
  case 0xD6:
    s = "HALT\0";
    break;
  case 0xD8:
    s = "STW\0";
    break;
  case 0xD9:
    s = "LDW\0";
    break;
  case 0xE0:
    s = "ADD\0";
    break;
  case 0xE1:
    s = "SUB\0";
    break;
  case 0xE2:
    s = "AND\0";
    break;
  case 0xE3:
    s = "OR\0";
    break;
  case 0xE4:
    s = "XOR\0";
    break;
  case 0xE5:
    s = "NOT\0";
    break;
  case 0xE6:
    s = "SLL\0";
    break;
  case 0xE7:
    s = "SRL\0";
    break;
  case 0xF8:
    s = "FADD\0";
    break;
  case 0xF9:
    s = "FMUL\0";
    break;
  case 0xFA:
    s = "FINV\0";
    break;
  case 0xFE:
    s = "FSQRT\0";
    break;
  case 0xFF:
    s = "FCMP\0";
    break;
  default:
    s = "----\0";
    break;
  }
  return s;
}

void printdecode(decode_result_type d){
  fprintf(stderr,"---------------------DECODE RESULT----------------------");
  fprintf(stderr,"\nop:  %s(%d)\n",deco(d.opc),d.opc);
  fprintf(stderr,"rt:  %d\n",d.rt);
  fprintf(stderr,"ra:  %d\n",d.ra);
  fprintf(stderr,"rb:  %d\n",d.rb);
  fprintf(stderr,"imm: %d\n",d.imm);
  fprintf(stderr,"--------------------------------------------------------\n\n");
}
