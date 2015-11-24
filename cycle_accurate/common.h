#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define HALT 0xD6

//bramもここにつっこんだ

//std_logic_vectorは全てuint32_tで表現

#define RS_NUM_WIDTH 2
typedef uint32_t rs_num_type;//vector 1 downto 0
#define PC_WIDTH 14
typedef uint32_t pc_type;//vec 13 downto 0
#define ROB_NUM_WIDTH 4
typedef uint32_t rob_num_type;// vec 3 downto 0
#define REG_NUM_WIDTH 5
typedef uint32_t reg_num_type;// vec 4 downto 0
#define REG_NUM_ZERO 15
typedef int opc_type;//代数的データ型
#define NOP 0
#define LIMM 0xD0
#define CMP 0xD1
#define IN 0xD2
#define OUT 0xD3
#define J 0xD4
#define JR 0xD5
#define JREQ 0xF0
#define JRNEQ 0xF1
#define JRGT 0xF2
#define JRGTE 0xF3
#define JRLT 0xF4
#define JRLTE 0xF5
#define STW 0xD8
#define LDW 0xD9
#define ADD 0xE0
#define SUB 0xE1
#define AND 0xE2
#define OR 0xE3
#define XOR 0xE4
#define NOT 0xE5
#define SLL 0xE6
#define SRL 0xE7
#define FADD 0xF8
#define FMUL 0xF9
#define FINV 0xFA
#define FSQRT 0xFE
#define FCMP 0xFF
typedef int unit_type;//代数的データ型
#define ALU_UNIT 1
#define FPU_UNIT 2
#define MEM_UNIT 3
#define BRANCH_UNIT 4
#define NULL_UNIT 0

#define GT_CONST 2
#define EQ_CONST 1
#define LT_CONST 0

typedef struct{
  int valid;//std_logic
  rob_num_type rob_num;
}rs_tag_type;

extern rs_tag_type rs_tag_zero;
#define RS_TAG_ZERO {0,0}

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

extern decode_result_type decode_result_zero;
#define DECODE_RESULT_ZERO {NOP,REG_NUM_ZERO,REG_NUM_ZERO,REG_NUM_ZERO,0,0,0,0}

typedef struct{
  uint32_t data;
  rs_tag_type tag;
}register_type;

extern register_type register_zero;
#define REGISTER_ZERO {0,RS_TAG_ZERO}

//regitser_array_typeは未定義

typedef struct{
  uint32_t data;//31 downto 0
  rs_tag_type tag;
  pc_type pc_next;
}cdb_type;

extern cdb_type cdb_zero;
#define CDB_ZERO {0,RS_TAG_ZERO,0}

typedef int rs_state_type;
#define RS_INVALID 0
#define RS_WAITING 1
#define RS_EXECUTING 2
#define RS_DONE 3
#define RS_RESERVED 4

typedef struct{
  rs_state_type state;
  register_type ra;
  register_type rb;
  uint32_t result;//31 downto 0
  rob_num_type rob_num;
  pc_type pc;
  pc_type pc_next;
}rs_common_type;

extern rs_common_type rs_common_zero;
#define RS_COMMON_ZERO {RS_INVALID,REGISTER_ZERO,REGISTER_ZERO,0,0,0,0}

typedef int rob_state_type;//Invalid->0
#define ROB_INVALID 0
#define ROB_EXECUTING 1
#define ROB_DONE 2
#define ROB_RESET 3
#define ROB_DUMMY 4

typedef struct{
  rob_state_type state;
  pc_type pc_next;
  uint32_t result;//31 downto 0
  reg_num_type reg_num;
}rob_type;

extern rob_type rob_zero;
#define ROB_ZERO {0,0,0,0}

typedef int sramif_op;//NOP->0

#define SRAM_NOP 0
#define SRAM_LOAD 1
#define SRAM_STORE 2

typedef struct{
  sramif_op op;
  uint32_t addr;//19 downto 0
  uint32_t wd;//31 downto 0
}sramif_in;

extern sramif_in sramif_in_zero;
#define SRAMIF_IN_ZERO {0,0,0}

typedef struct{
  int data_enable;//std_logic
  uint32_t rd;
}sramif_out;

extern sramif_out sramif_out_zero;
#define SRAMIF_OUT_ZERO {0,0}

typedef struct{
  int rd_en;//std_logic
}recvif_in_type;

extern recvif_in_type recvif_in_zero;
#define RECVIF_IN_ZERO {0}

typedef struct{
  uint32_t dout;//7 downto 0
  int full;//std_logic
  int empty;//std_logic
}recvif_out_type;

extern recvif_out_type recvif_out_zero;
#define RECVIF_OUT_ZERO {0,0,0}

typedef struct{
  int wr_en;//std_logic
  uint32_t din;//7 downto 0
}transif_in_type;

extern transif_in_type transif_in_zero;
#define TRANSIF_IN_ZERO {0,0}

typedef struct{
  int full;//std_logic
}transif_out_type;

extern transif_in_type transif_out_zero;
#define TRANSIF_OUT_ZERO {0}

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

extern int rs_common_ready(rs_common_type r);

extern int my_equal(rs_tag_type a, rs_tag_type b);

extern register_type register_update(register_type reg, cdb_type cdb);

extern cdb_type make_cdb_out(rs_common_type rs_common);

/***********************
  alu pack

 ***********************/

typedef int alu_op_type;//代(ry

#define LIMM_OP 0xD0
#define CMP_OP 0xD1
#define ADD_OP 0xE0
#define SUB_OP 0xE1
#define AND_OP 0xE2
#define OR_OP 0xE3
#define XOR_OP 0xE4
#define NOT_OP 0xE5
#define SLL_OP 0xE6
#define SRL_OP 0xE7

#define ALU_NOP_OP 0

typedef struct{
  alu_op_type op;
  rs_common_type common;
}alu_rs_type;

extern alu_rs_type alu_rs_zero;
#define ALU_RS_ZERO {ALU_NOP_OP,RS_COMMON_ZERO}

//alu_rs_arrayはmallocがめんどいのであとで配列宣言

typedef struct{
  alu_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}alu_in_type;

extern alu_in_type alu_in_zero;
#define ALU_IN_ZERO {ALU_RS_ZERO,CDB_ZERO,0,0}

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}alu_out_type;

extern alu_out_type alu_out_zero;
#define ALU_OUT_ZERO {0,CDB_ZERO}

/***********************
  fpu pack

 ***********************/

typedef int fpu_op_type;//代(ry

#define FADD_OP 0xF8
#define FMUL_OP 0xF9
#define FINV_OP 0xFA
#define FSQRT_OP 0xFE
#define FCMP_OP 0xFF

#define FPU_NOP_OP 0

typedef struct{
  fpu_op_type op;
  int countdown;//vec 2 downto 0
  rs_common_type common;
}fpu_rs_type;

extern fpu_rs_type fpu_rs_zero;
#define FPU_RS_ZERO {FPU_NOP_OP,0,RS_COMMON_ZERO}

//fpu_rs_arrayはmallocがめんどいのであとで配列宣言

typedef struct{
  fpu_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}fpu_in_type;

extern fpu_in_type fpu_in_zero;
#define FPU_IN_ZERO {FPU_RS_ZERO,CDB_ZERO,0,0}

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}fpu_out_type;

extern fpu_out_type fpu_out_zero;
#define FPU_OUT_ZERO {0,CDB_ZERO}


/***********************
  mem pack

 ***********************/

typedef int mem_op_type;//代(ry
//serial通信も受け持つ
#define IN_OP 0xD2
#define OUT_OP 0xD3

#define STORE 0xD8
#define LOAD 0xD9

#define MEM_NOP_OP 0

typedef struct{
  mem_op_type op;
  int has_dummy;//std_logic
  rs_common_type common;
}mem_rs_type;

extern mem_rs_type mem_rs_zero;
#define MEM_RS_ZERO {MEM_NOP_OP,0,RS_COMMON_ZERO}

//mem_rs_arrayはmallocがめんどいのであとで配列宣言

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

extern mem_in_type mem_in_zero;
#define MEM_IN_ZERO {MEM_RS_ZERO,CDB_ZERO,0,0,0,SRAMIF_OUT_ZERO,RECVIF_OUT_ZERO,TRANSIF_OUT_ZERO}

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
  sramif_in sramifin;
  recvif_in_type recvifin;
  transif_in_type transifin;
}mem_out_type;

extern mem_out_type mem_out_zero;
#define MEM_OUT_ZERO {0,CDB_ZERO,SRAMIF_IN_ZERO,RECVIF_IN_ZERO,TRANSIF_IN_ZERO}


/***********************
  branch pack

 ***********************/

typedef int branch_op_type;//代(ry

#define J_OP 0xD4
#define JR_OP 0xD5
#define JREQ_OP 0xF0
#define JRNEQ_OP 0xF1
#define JRGT_OP 0xF2
#define JRGTE_OP 0xF3
#define JRLT_OP 0xF4
#define JRLTE_OP 0xF5

#define BRANCH_NOP_OP 0

typedef struct{
  branch_op_type op;
  rs_common_type common;
}branch_rs_type;

extern branch_rs_type branch_rs_zero;
#define BRANCH_RS_ZERO {BRANCH_NOP_OP,RS_COMMON_ZERO}

//branch_rs_arrayはmallocがめんどいのであとで配列宣言

typedef struct{
  branch_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}branch_in_type;

extern branch_in_type branch_in_zero;
#define BRANCH_IN_ZERO {BRANCH_RS_ZERO,CDB_ZERO,0,0}

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}branch_out_type;

extern branch_out_type branch_out_zero;
#define BRANCH_OUT_ZERO {0,CDB_ZERO}

extern uint32_t bram[262144];//1MB memory

extern uint32_t sram[1048576];//4MB memory

extern uint32_t blockram(int clk, int we, uint32_t addr, uint32_t din);

extern sramif_out sramif(int clk, int rst, sramif_in sramifin);

//extern uint32_t/*7 dto 0*/ recvif(int clk, int rst, int rd_en,  sramifin);

extern void program_loader(char* binary);

extern void printdecode(decode_result_type d);

extern char* deco(int o);

extern char* deco_rs_state(int o);

extern cpu_top_out_type cpu_top(int clk, int rst, cpu_top_in_type cpu_top_in);
