#include "fpu.h"
#include "common.h"

typedef struct{
  fpu_rs_type rs[4];
  int rs_full;//std_logic
  rs_num_type free_rs_num;
  cdb_type cdb_out;
  rs_num_type cdb_rs_num;
}fpu_reg_type;

fpu_reg_type f_reg_zero = {{FPU_RS_ZERO,FPU_RS_ZERO,FPU_RS_ZERO,FPU_RS_ZERO},0,0,CDB_ZERO,0};
#define F_REG_ZERO {{FPU_RS_ZERO,FPU_RS_ZERO,FPU_RS_ZERO,FPU_RS_ZERO},0,0,CDB_ZERO,0}

fpu_reg_type f_r= F_REG_ZERO;
fpu_reg_type f_r_in= F_REG_ZERO;

//op
uint32_t fadd_op1;
uint32_t fadd_op2;
uint32_t fmul_op1;
uint32_t fmul_op2;
uint32_t fsqrt_op1;
uint32_t finv_op1;

//answer queue
uint32_t fadd_ans[2];
uint32_t fmul_ans[2];
uint32_t fsqrt_ans[3];
uint32_t finv_ans[3];

fpu_out_type fpu(int clk, int rst, fpu_in_type fpu_in){//twoproc

  fpu_reg_type v = f_r;
  uint32_t ra_data;
  uint32_t rb_data;
  int fadd_used;//bool
  uint32_t fadd_op1_v, fadd_op2_v;
  int fmul_used;//bool
  uint32_t fmul_op1_v, fmul_op2_v;
  int fsqrt_used;//bool
  uint32_t fsqrt_op1_v;
  int finv_used;//bool
  uint32_t finv_op1_v;

  int i=0;
  
  v = f_r;
  //update rs
  for(i=0;i<4;i++){
    v.rs[i].common.ra = register_update(f_r.rs[i].common.ra, fpu_in.cdb_in);
    v.rs[i].common.rb = register_update(f_r.rs[i].common.rb, fpu_in.cdb_in);
  }

  fadd_used = 0;//false;
  fmul_used = 0;//false;
  fsqrt_used = 0;//false;
  finv_used = 0;//false;
  fadd_op1_v = 0;//(others => '0');
  fadd_op2_v = 0;//(others => '0');
  fmul_op1_v = 0;//(others => '0');
  fmul_op2_v = 0;//(others => '0');
  fsqrt_op1_v = 0;//(others => '0');
  finv_op1_v = 0;//(others => '0');
  
  for(i=0;i<4;i++){
    if(rs_common_ready(f_r.rs[i].common)){
      ra_data = f_r.rs[i].common.ra.data;
      rb_data = f_r.rs[i].common.rb.data;
      switch(f_r.rs[i].op){
      case FADD:
	if(!fadd_used){
	  fadd_op1_v = ra_data;
	  fadd_op2_v = rb_data;
	  fadd_used = 1;//true;
	  v.rs[i].common.state = RS_EXECUTING;
	  v.rs[i].countdown = 0;
	}
	break;
      case FMUL:
	if(!fmul_used){
	  fmul_op1_v = ra_data;
	  fmul_op2_v = rb_data;
	  fmul_used = 1;//true;
	  v.rs[i].common.state = RS_EXECUTING;
	  v.rs[i].countdown = 1;
	}
	break;
      case FINV:
	if(!finv_used){
	  finv_op1_v = ra_data;
	  finv_used = 1;//true;
	  v.rs[i].common.state = RS_EXECUTING;
	  v.rs[i].countdown = 1;
	}
	break;
      case FSQRT:
	if(!fsqrt_used){
	  fsqrt_op1_v = ra_data;
	  fsqrt_used = 1;//true;
	  v.rs[i].common.state = RS_EXECUTING;
	  v.rs[i].countdown = 1;
	}
	break;
      case FCMP://VHDL internal operation
	v.rs[i].common.result = fcmp(ra_data,rb_data);//but on library here
	v.rs[i].common.state = RS_DONE;
	break;
      case NOP:
	break;
      }

    v.rs[i].common.pc_next = f_r.rs[i].common.pc + 1;
    }else if(f_r.rs[i].common.state == RS_EXECUTING){
      if(f_r.rs[i].countdown == 0){
	switch(f_r.rs[i].op){
	case FADD:
	  v.rs[i].common.result = fadd_ans[1];
	  v.rs[i].common.state = RS_DONE;
	  break;
	case FMUL:
	  v.rs[i].common.result = fmul_ans[1];
	  v.rs[i].common.state = RS_DONE;
	  break;
	case FINV:
	  v.rs[i].common.result = finv_ans[2];
	  v.rs[i].common.state = RS_DONE;
	  break;
	case FSQRT:
	  v.rs[i].common.result = fsqrt_ans[2];
	  v.rs[i].common.state = RS_DONE;
	  break;
	}
      }else
	v.rs[i].countdown = f_r.rs[i].countdown - 1;
    }
  }
  
  fadd_op1 = fadd_op1_v;
  fadd_op2 = fadd_op2_v;
  fmul_op1 = fmul_op1_v;
  fmul_op2 = fmul_op2_v;
  fsqrt_op1 = fsqrt_op1_v;
  finv_op1 = finv_op1_v;

  if(f_r.rs_full == 0 && fpu_in.rs_in.op != NOP)
    v.rs[f_r.free_rs_num] = fpu_in.rs_in;
  if(fpu_in.cdb_next == 1 || f_r.cdb_out.tag.valid == 0){
    // clear rs
    if(f_r.cdb_out.tag.valid == 1)
      v.rs[f_r.cdb_rs_num] = fpu_rs_zero;
    // select next rs for cdb
    v.cdb_out = cdb_zero;
    for(i=0;i<4;i++){
      if(v.rs[i].common.state == RS_DONE){
      v.cdb_out = make_cdb_out(v.rs[i].common);
      v.cdb_rs_num = i;
      }
    }
  }
  // update output (rs_full/free_rs_num)
  v.rs_full = 1;
  for(i=0;i<4;i++){
    if(v.rs[i].common.state == RS_INVALID){
      v.free_rs_num = i;
      v.rs_full = 0;
    }
  }

  //external calculation
  fadd_ans[1] = fadd_ans[0];
  fmul_ans[1] = fmul_ans[0];
  fsqrt_ans[2] = fsqrt_ans[1];
  fsqrt_ans[1] = fsqrt_ans[0];
  finv_ans[2] = finv_ans[1];
  finv_ans[1] = finv_ans[0];
  
  fadd_ans[0] = fadd(fadd_op1,fadd_op2);
  fmul_ans[0] = fmul(fmul_op1,fmul_op2);
  fsqrt_ans[0] = fsqrt(fsqrt_op1);
  finv_ans[0] = finv(finv_op1);

  //clk rising
  if(rst)
    v = f_reg_zero;
  else
    f_r = f_r_in;

  if(fpu_in.rst == 1)
    f_r_in=f_reg_zero;
  else
    f_r_in=v;

  fpu_out_type fpu_out={f_r.rs_full,f_r.cdb_out};

  return fpu_out;
}
