#include "common.h"

typedef struct{
  alu_rs_type rs[4];
  int rs_full;//std_logic
  rs_num_type free_rs_num;
  cdb_type cdb_out;
  rs_num_type cdb_rs_num;
}reg_type;

reg_type reg_zero = {{alu_rs_zero,alu_rs_zero,alu_rs_zero,alu_rs_zero},0,0,cdb_zero,0};

//procedure find_free_rsはポインタ渡しvoid型で実装

alu_out_type alu(int clk, int rst, alu_in_type alu_in){
  
  
