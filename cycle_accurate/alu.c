#include "common.h"
#include <stdint.h>
#include <stdio.h>

typedef struct{
  alu_rs_type rs[4];
  int rs_full;//std_logic
  rs_num_type free_rs_num;
  cdb_type cdb_out;
  rs_num_type cdb_rs_num;
}alu_reg_type;

alu_reg_type a_reg_zero = {{ALU_RS_ZERO,ALU_RS_ZERO,ALU_RS_ZERO,ALU_RS_ZERO},0,0,CDB_ZERO,0};
#define REG_ZERO {{ALU_RS_ZERO,ALU_RS_ZERO,ALU_RS_ZERO,ALU_RS_ZERO},0,0,CDB_ZERO,0}

alu_reg_type a_r = REG_ZERO;
alu_reg_type a_r_in = REG_ZERO;

//procedure find_free_rsはポインタ渡しvoid型で実装->コア曰くやっぱりいらない

extern char* deco(int o);
extern char* deco_rs_state(int o);

void printcdb(cdb_type cdb){
  char* v = malloc(sizeof(char)*3);
  if(cdb.tag.valid)
    v = "OK\0";
  else
    v = "NG\0";
  fprintf(stderr,"\ncdb: data        tag          pc_next\n");
  fprintf(stderr,"cdb: %-8d    %-2d(rob)      %-8d    %s\n\n",cdb.data,cdb.tag.rob_num,cdb.pc_next,v);
}

void printrs(alu_reg_type ar){
  int i=0;
  char* inst;
  char* state;
  char* v1 = malloc(sizeof(char)*3);
  char* v2 = malloc(sizeof(char)*3);
  for(i=0;i<4;i++){
    inst = deco(ar.rs[i].op);
    state = deco_rs_state(ar.rs[i].common.state);
    if(ar.rs[i].common.ra.tag.valid)
      v1 = "NG\0";
    else
      v1 = "OK\0";
    if(ar.rs[i].common.rb.tag.valid)
      v2 = "NG\0";
    else
      v2 = "OK\0";
    fprintf(stderr,"ALUrs(%d): inst  state   rt        ra        rb        pc      pcnext\n",i);
    fprintf(stderr,"addr    : %-5s %-4s    %-2d(rob)   %-2d(rob)   %-2d(rob)\n",inst,state,ar.rs[i].common.rob_num,ar.rs[i].common.ra.tag.rob_num,ar.rs[i].common.rb.tag.rob_num);
    fprintf(stderr,"value   :               %-6d    %-6d    %-6d    %-7d %-7d \n",ar.rs[i].common.result,ar.rs[i].common.ra.data,ar.rs[i].common.rb.data,ar.rs[i].common.pc,ar.rs[i].common.pc_next);
    fprintf(stderr,"validity:                         %s        %s\n",v1,v2);
    fprintf(stderr,"\n");
    /*free(inst);
    free(state);
    free(v1);
    free(v2);*/
  }
}

alu_out_type alu(int clk, int rst, alu_in_type alu_in){
  
  alu_reg_type v;
  int exec_done;//boolean
  uint32_t ra_data;//vec 31 downto 0
  uint32_t rb_data;//vec 31 downto 0

  v=a_r;
  
  //update rs
  int i;
  for(i=0;i<4;i++){
    v.rs[i].common.ra = register_update(a_r.rs[i].common.ra, alu_in.cdb_in);
    v.rs[i].common.rb = register_update(a_r.rs[i].common.rb, alu_in.cdb_in);
  }
  
  //execute
  exec_done = 0;
  for(i=0;i<4;i++){
    if(rs_common_ready(v.rs[i].common) && !(exec_done)){
      ra_data = v.rs[i].common.ra.data;
      rb_data = v.rs[i].common.rb.data;
      switch(v.rs[i].op){
      case LIMM:
	v.rs[i].common.result = ra_data;
	break;
      case ADD:
	v.rs[i].common.result = ra_data + rb_data;
	break;
      case SUB:
	v.rs[i].common.result = ra_data - rb_data;
	break;
      case AND:
	v.rs[i].common.result = ra_data & rb_data;
	break;
      case OR:
	v.rs[i].common.result = ra_data | rb_data;
	break;
      case XOR:
	v.rs[i].common.result = ra_data ^ rb_data;
	break;
      case NOT:
	v.rs[i].common.result = ~ra_data;
	break;
      case SLL:
	v.rs[i].common.result = ra_data << 1;
	break;
      case SRL:
	v.rs[i].common.result = ra_data >> 1;
	break;
      case CMP:
	if(ra_data > rb_data)
	  v.rs[i].common.result = GT_CONST;
	else if(ra_data == rb_data)
	  v.rs[i].common.result = EQ_CONST;
	else
	  v.rs[i].common.result = LT_CONST;
	break;
      default:
	fprintf(stderr,"undefined operator\n");
	break;
      }
      v.rs[i].common.state = RS_DONE;
      v.rs[i].common.pc_next = v.rs[i].common.pc+1;//without cast
      exec_done=1;
    }
  }
  
  //store new rs contents
  if(a_r.rs_full == 0 && alu_in.rs_in.op != NOP)
    v.rs[a_r.free_rs_num] = alu_in.rs_in;
  
  //update output (cdb)
  if(alu_in.cdb_next == 1 || a_r.cdb_out.tag.valid == 0){
    //clear rs
    if(a_r.cdb_out.tag.valid == 1)
      v.rs[a_r.cdb_rs_num] = alu_rs_zero;
    
    //select next rs for cdb
    v.cdb_out = cdb_zero;
    for(i=0;i<4;i++){
      if(v.rs[i].common.state == RS_DONE){
	v.cdb_out = make_cdb_out(v.rs[i].common);
	v.cdb_rs_num = i;//without cast
      }
    }
  }
  
  v.rs_full = 1;
  for(i=0;i<4;i++){
    if(v.rs[i].common.state == RS_INVALID){
      v.free_rs_num = i;//without cast
      v.rs_full = 0;
    }
  }

  //clk rising
  if(rst)
    a_r=a_reg_zero;
  else
    a_r=a_r_in;
  
  if(alu_in.rst == 1)
    a_r_in=a_reg_zero;
  else
    a_r_in=v;

  alu_out_type alu_out={a_r.rs_full,a_r.cdb_out};

  printrs(a_r);

  printcdb(alu_out.cdb_out);

  return alu_out;
}
