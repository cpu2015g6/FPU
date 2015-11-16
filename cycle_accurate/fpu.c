#include "common.h"
#include "fpu.h"

typedef struct{
  fpu_rs_type rs[4];
  int rs_full;//std_logic
  rs_num_type free_rs_num;
  cdb_type cdb_out;
  rs_num_type cdb_rs_num;
}reg_type;

reg_type reg_zero = {{fpu_rs_zero,fpu_rs_zero,fpu_rs_zero,fpu_rs_zero},0,0,cdb_zero,0};

reg_type r,r_in=reg_zero;

fpu_out_type fpu(int clk, int rst, fpu_in_type fpu_in){//twoproc
  if(rst)
    v = reg_zero;
  else
    r = r_in;

  reg_type v = r;
  
