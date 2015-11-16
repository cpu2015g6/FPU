//std_logic_vectorは全てuint32_tで表現

#define RS_NUM_WIDTH 2
typedef rs_num_type uint32_t;//vector 1 downto 0
#define PC_WIDTH 14
typedef pc_type uint32_t;//vec 13 downto 0
#define ROB_NUM_WIDTH 4
typedef rob_num_type uint32_t;// vec 3 downto 0
#define REG_NUM_WIDTH 5
typedef reg_num_type uint32_t;// vec 4 downto 0
#define REG_NUM_ZERO 15
typedef opc_type int;//代数的データ型
#define NOP 0
#define LIMM 1
#define CMP 2
#define IN 3
#define OUT 4
#define J 5
#define JR 6
#define JREQ 7
#define JRNEQ 8
#define JRGT 9
#define JRGTE 10
#define JRLT 11
#define JRLTE 12
#define STW 13
#define LDW 14
#define ADD 15
#define SUB 16
#define AND 17
#define OR 18
#define XOR 19
#define NOT 20
#define SLL 21
#define SRL 22
#define FADD 23
#define FMUL 24
#define FINV 25
#define FSQRT 26
#define FCMP 27
typedef unit_type int;//代数的データ型
#define ALU 1
#define FPU 2
#define MEM 3
#define BRANCH 4
#define NULL 0

#define GT_CONST 2
#define EQ_CONST 1
#define LT_CONST 0

#define RS_INVALID 0
#define RS_WAITING 1
#define RS_EXECUTING 2
#define RS_DONE 3
#define RS_RESERVED 4

typedef struct{
  int valid;//std_logic
  rob_num_type rob_num;
}rs_tag_type;

rs_tag_type rs_tag_zero = {0,0};

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

decode_result_type decode_result_zero={NOP,reg_num_zero,reg_num_zero,reg_num_zero,0,0,0,0};

typedef struct{
  uint32_t data;
  rs_tag_type tag;
}register_type;

register_type register_zero ={0,rs_tag_zero};

typedef struct{
  register_type array[32];
}register_array_type;

register_array_type register_array_zero={register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero,register_zero};//Cはクソ

typedef struct{
  uint32_t data;//31 downto 0
  rs_tag_type tag;
  pc_type pc_next;
}cdb_type;

cdb_type cdb_zero = {0,rs_tag_zero,0};

typedef rs_state_type int;//Invalid->0

typedef struct{
  rs_state_type state;
  register_type ra;
  register_type rb;
  uint32_t result;//31 downto 0
  rob_num_type rob_num;
  pc_type pc;
  pc_type pc_next;
}rs_common_type;

rs_common_type rs_common_zero = {RS_INVALID,register_zero,register_zero,0,0,0,0};

typedef rob_state_type int;//Invalid->0

typedef struct{
  rob_state_type state;
  pc_type pc_next;
  uint32_t result;//31 downto 0
  reg_num_type reg_num;
}rob_type;

rob_type rob_zero={0,0,0,0};

typedef sramif_op int;//NOP->0

typedef struct{
  sramif_op op;
  uint32_t addr;//19 downto 0
  uint32_t wd;//31 downto 0
}sramif_in;

sramif_in sramif_in_zero={0,0,0};

typedef struct{
  int data_enable;//std_logic
  uint32_t rd;
}sramif_out;

sramif_out sramif_out_zero={0,0};

typedef struct{
  int rd_en;//std_logic
}recvif_in_type;

recvif_in_type recvif_in_zero={0};

typedef struct{
  uint32_t dout;//7 downto 0
  int full;//std_logic
  int empty;//std_logic
}recvif_out_type;

recvif_out_type recvif_out_zero={0,0,0};

typedef struct{
  int wr_en;//std_logic
  uint32_t din;//7 downto 0
}transif_in_type;

transif_in_type transif_in_zero={0,0};

typedef struct{
  int full;//std_logic
}transif_out_type;

transif_in_type transif_out_zero={0};

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

int rs_common_ready(rs_common_type r){//booleanの代わりにint
  if(r.state == RS_WAITING && r.ra.tag.valid == 0 && r.rb.tag.valid == 0)
    return 1;
  return 0;
}

register_type register_update(register_type reg, cdb_type cdb){
  register_type v;
  if(cdb.tag.valid == 1 && cdb.tag == reg.tag){
    v.tag=rs_tag_zero;
    v.data=cdb.data;
  }else
    v=reg;
  return v;
}

cdb_type make_cdb_out(rs_common_type rs_common){
  cdb_type c;
  c.tag.valid=1;
  c.tag.rob_num=rs.common.rob.num;
  c.data=rs_common_result;
  c.pc_next=rs_common.pc_next;
  return c;
}

/***********************
  alu pack

 ***********************/

typedef alu_op_type int;//代(ry

#define LIMM_OP 1
#define CMP_OP 2
#define ADD_OP 13
#define SUB_OP 14
#define AND_OP 15
#define OR_OP 16
#define XOR_OP 17
#define NOT_OP 18
#define SLL_OP 19
#define SRL_OP 20

#define ALU_NOP_OP 0

typedef struct{
  alu_op_type op;
  rs_common_type common;
}alu_rs_type;

alu_rs_type alu_rs_zero={ALU_NOP_OP,rs_common_zero};

//alu_rs_arrayはmallocがめんどいのであとで配列宣言

typedef struct{
  rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}alu_in_type;

alu_in_type alu_in_zero = {alu_rs_zero,cdb_zero,0,0};

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}alu_out_type;

alu_out_type alu_out_zero = {0,cdb_zero};

/***********************
  fpu pack

 ***********************/

typedef fpu_op_type int;//代(ry

#define FADD 21
#define FMUL 22
#define FINV 23
#define FSQRT 24
#define FCMP 25
#define FPU_NOP_OP 0

typedef struct{
  fpu_op_type op;
  int countdown;//vec 2 downto 0
  rs_common_type common;
}fpu_rs_type;

fpu_rs_type fpu_rs_zero={FPU_NOP_OP,0,rs_common_zero};

//fpu_rs_arrayはmallocがめんどいのであとで配列宣言

typedef struct{
  fpu_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}fpu_in_type;

fpu_in_type fpu_in_zero = {fpu_rs_zero,cdb_zero,0,0};

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}fpu_out_type;

fpu_out_type fpu_out_zero = {0,cdb_zero};


/***********************
  mem pack

 ***********************/

typedef mem_op_type int;//代(ry
//serial通信も受け持つ
#define IN 3
#define OUT 4

#define STW 13
#define LDW 14

#define MEM_NOP_OP 0

typedef struct{
  mem_op_type op;
  int has_dummy;//std_logic
  rs_common_type common;
}mem_rs_type;

mem_rs_type mem_rs_zero={MEM_NOP_OP,0,rs_common_zero};

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

mem_in_type mem_in_zero = {mem_rs_zero,cdb_zero,0,0,0,sramif_out_zero,recvif_out_zero,transif_out_zero};

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
  sramif_in sramifin;
  recvif_in_type recvifin;
  transif_in_type transifin;
}mem_out_type;

mem_out_type mem_out_zero = {0,cdb_zero,sramif_in_zero,recvif_in_zero,transif_in_zero};


/***********************
  branch pack

 ***********************/

typedef branch_op_type int;//代(ry

#define J 5
#define JR 6
#define JREQ 7
#define JRNEQ 8
#define JRGT 9
#define JRGTE 10
#define JRLT 11
#define JRLTE 12

#define BRANCH_NOP_OP 0

typedef struct{
  branch_op_type op;
  rs_common_type common;
}branch_rs_type;

branch_rs_type branch_rs_zero={BRANCH_NOP_OP,rs_common_zero};

//beanch_rs_arrayはmallocがめんどいのであとで配列宣言

typedef struct{
  mem_rs_type rs_in;
  cdb_type cdb_in;
  int cdb_next;//std_logic
  int rst;//std_logic
}branch_in_type;

branch_in_type branch_in_zero = {branch_rs_zero,cdb_zero,0,0};

typedef struct{
  int rs_full;//std_logic
  cdb_type cdb_out;
}branch_out_type;

branch_out_type branch_out_zero = {0,cdb_zero};
