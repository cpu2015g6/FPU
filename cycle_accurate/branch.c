#include "common.h"

typedef struct{
  branch_rs_type rs[4];
  int rs_full;//std_logic
  rs_num_type free_rs_num;
  cdb_type cdb_out;
  rs_num_type cdb_rs_num;
}branch_reg_type;

branch_reg_type b_reg_zero = {{BRANCH_RS_ZERO,BRANCH_RS_ZERO,BRANCH_RS_ZERO,BRANCH_RS_ZERO},0,0,CDB_ZERO,0};
#define B_REG_ZERO {{BRANCH_RS_ZERO,BRANCH_RS_ZERO,BRANCH_RS_ZERO,BRANCH_RS_ZERO},0,0,CDB_ZERO,0}

branch_reg_type b_r = B_REG_ZERO;
branch_reg_type b_r_in = B_REG_ZERO;

branch_out_type branch(int clk, int rst, branch_in_type branch_in){
  
  branch_reg_type v;
  int exec_done;//boolean
  uint32_t pc32;
  
  int i=0;

  v=b_r;
  
  for(i=0;i<4;i++){
    v.rs[i].common.ra = register_update(b_r.rs[i].common.ra,branch_in.cdb_in);
    v.rs[i].common.rb = register_update(b_r.rs[i].common.rb,branch_in.cdb_in);
  }
  
  exec_done = 0;//false
  
  for(i=0;i<4;i++){
    if(rs_common_ready(v.rs[i].common) && !(exec_done)){
      pc32 = v.rs[i].common.pc + 1;
      switch(v.rs[i].op){
      case J://相対
	v.rs[i].common.pc_next = v.rs[i].common.pc + v.rs[i].common.ra.data;
	v.rs[i].common.result = pc32;
	break;
      case JR://絶対
	v.rs[i].common.pc_next = v.rs[i].common.ra.data;
	v.rs[i].common.result = pc32;
	break;
      case JREQ:
	if(v.rs[i].common.ra.data == EQ_CONST)
	  v.rs[i].common.pc_next = v.rs[i].common.rb.data;
	else
	  v.rs[i].common.pc_next = v.rs[i].common.pc + 1;
	v.rs[i].common.result = pc32;
	break;
      case JRNEQ:
	if(v.rs[i].common.ra.data != EQ_CONST)
	  v.rs[i].common.pc_next = v.rs[i].common.rb.data;
	else
	  v.rs[i].common.pc_next = v.rs[i].common.pc + 1;
	v.rs[i].common.result = pc32;
	break;
      case JRGT:
	if(v.rs[i].common.ra.data == GT_CONST)
	  v.rs[i].common.pc_next = v.rs[i].common.rb.data;
	else
	  v.rs[i].common.pc_next = v.rs[i].common.pc + 1;
	v.rs[i].common.result = pc32;
	break;
      case JRGTE:
	if(v.rs[i].common.ra.data != LT_CONST)
	  v.rs[i].common.pc_next = v.rs[i].common.rb.data;
	else
	  v.rs[i].common.pc_next = v.rs[i].common.pc + 1;
	v.rs[i].common.result = pc32;
	break;
      case JRLT:
	if(v.rs[i].common.ra.data == LT_CONST)
	  v.rs[i].common.pc_next = v.rs[i].common.rb.data;
	else
	  v.rs[i].common.pc_next = v.rs[i].common.pc + 1;
	v.rs[i].common.result = pc32;
	break;
      case JRLTE:
	if(v.rs[i].common.ra.data != GT_CONST)
	  v.rs[i].common.pc_next = v.rs[i].common.rb.data;
	else
	  v.rs[i].common.pc_next = v.rs[i].common.pc + 1;
	v.rs[i].common.result = pc32;
	break;
      case NOP:
	break;
      }
      v.rs[i].common.state = RS_DONE;
      exec_done = 1;
    }
  }
  
  if(b_r.rs_full == 0 && branch_in.rs_in.op != NOP)
    v.rs[b_r.free_rs_num] = branch_in.rs_in;

  if(branch_in.cdb_next == 1 || b_r.cdb_out.tag.valid == 0){
    // clear rs
    if(b_r.cdb_out.tag.valid == 1)
      v.rs[b_r.cdb_rs_num] = branch_rs_zero;
    // select next rs for cdb
    v.cdb_out = cdb_zero;
    for(i=0;i<4;i++){
      if(v.rs[i].common.state == RS_DONE){
	v.cdb_out = make_cdb_out(v.rs[i].common);
	v.cdb_rs_num = i;
      }
    }
  }
  
  v.rs_full = 1;

  for(i=0;i<4;i++){
    if(v.rs[i].common.state == RS_INVALID){
      v.free_rs_num = i;
      v.rs_full = 0;
    }
  }

  if(rst)
    b_r=b_reg_zero;
  else
    b_r=b_r_in;
  
  //synchronous reset
  if(branch_in.rst == 1)
    b_r_in = b_reg_zero;
  else
    b_r_in = v;

  branch_out_type branch_out={b_r.rs_full,b_r.cdb_out};

  return branch_out;
}
