#include "common.h"
#include <stdint.h>
#include <string.h>

typedef struct{
  mem_rs_type rs[4];
  uint32_t countdown[4];
  rs_num_type rs_youngest;
  rs_num_type rs_oldest;
  rs_num_type rs_exec;
  int rs_full;//std_logic
  cdb_type cdb_out;
  int dummy_done;//std_logic
}mem_reg_type;

mem_reg_type m_reg_zero = {{MEM_RS_ZERO,MEM_RS_ZERO,MEM_RS_ZERO,MEM_RS_ZERO},{0,0,0,0},0,0,0,0,CDB_ZERO,0};
#define M_REG_ZERO {{MEM_RS_ZERO,MEM_RS_ZERO,MEM_RS_ZERO,MEM_RS_ZERO},{0,0,0,0},0,0,0,0,CDB_ZERO,0}

mem_reg_type m_r = M_REG_ZERO;
mem_reg_type m_r_in = M_REG_ZERO;

mem_rs_type rs_executing_update(mem_rs_type rs, uint32_t load_in, uint32_t serial_in){
  mem_rs_type new_rs;
  switch(rs.op){
  case LOAD:
    new_rs.common.result = load_in;
    break;
  case IN:
    new_rs.common.result = serial_in;
    break;
  }
  new_rs.common.state = RS_DONE;
  new_rs.common.pc_next = rs.common.pc + 1;
  return new_rs;
}

void print_mem_rs(mem_reg_type ar){
  int i=0;
  char* inst;
  char* state;
  char* v1 = malloc(sizeof(char)*3);
  char* v2 = malloc(sizeof(char)*3);
  fprintf(stderr,"\n-----------------------MEM reservasion station----------------------\n");
  for(i=0;i<4;i++){
    inst = deco(ar.rs[i].op);
    //fprintf(stderr,"op: %d",ar.rs[i].op);
    state = deco_rs_state(ar.rs[i].common.state);
    if(strcmp(inst,"----")){
    if(ar.rs[i].common.ra.tag.valid)
      v1 = "NG\0";
    else
      v1 = "OK\0";
    if(ar.rs[i].common.rb.tag.valid)
      v2 = "NG\0";
    else
      v2 = "OK\0";
    fprintf(stderr,"MEMrs(%d): inst  state   rt        ra        rb        pc      pcnext\n",i);
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
}

mem_out_type mem(int clk, int rst, mem_in_type mem_in/*mem_op_type rs_in_op,
		 int rs_in_has_dummy,
		 rs_common_type rs_in_common,cdb_type cdb_in,
		 int cdb_next,int sync_rst,int dummy_done,
		 sramif_out sramifout,
		 recvif_out_type recvifout,
		 transif_out_type transifout*/){

  mem_out_type mem_out;

  /*mem_in.rs_in.op=rs_in_op;
  mem_in.rs_in.has_dummy=rs_in_has_dummy;
  mem_in.rs_in.common=rs_in_common;
  mem_in.cdb_in = cdb_in;
  mem_in.cdb_next = cdb_next;
  mem_in.rst = sync_rst;
  mem_in.dummy_done = dummy_done;
  mem_in.sramifout = sramifout;
  mem_in.recvifout = recvifout;
  mem_in.transifout = transifout;*/
  //mem_out.cdb_out = m_r.cdb_out;
  //mem_out.rs_full = m_r.rs_full;
  
  int i;

  mem_reg_type v;
  int exec_complete;//boolean
  rs_common_type t;
  sramif_in sramifin_v;
  transif_in_type transifin_v;
  recvif_in_type recvifin_v;
  int exec_i;

  v=m_r;
  if(mem_in.dummy_done == 1)
    v.dummy_done = 1;
  
  for(i=0;i<4;i++){
    v.rs[i].common.ra = register_update(m_r.rs[i].common.ra, mem_in.cdb_in);
    v.rs[i].common.rb = register_update(m_r.rs[i].common.rb, mem_in.cdb_in);
    if(m_r.rs[i].common.state == RS_EXECUTING){
      if(v.countdown[i] == 0)
	v.rs[i] = rs_executing_update(m_r.rs[i], 
				      mem_in.sramifout.rd, 
				      mem_in.recvifout.dout);
      else
	v.countdown[i] = v.countdown[i] - 1;
    }
  }

  exec_complete = 0;//false
  sramifin_v = sramif_in_zero;
  recvifin_v = recvif_in_zero;
  transifin_v = transif_in_zero;
  exec_i = m_r.rs_exec;
  t = m_r.rs[exec_i].common;

  if(rs_common_ready(t)){
    if(m_r.rs[exec_i].has_dummy == 0 || m_r.dummy_done == 1)
      exec_complete = 1;//true
    else
      exec_complete = 0;//false
    if(exec_complete){
      switch(m_r.rs[exec_i].op){
      case LOAD:
	v.rs[exec_i].common.state = RS_EXECUTING;
	v.countdown[exec_i] = 3;//"011"
	sramifin_v.op = SRAM_LOAD;
	sramifin_v.addr = t.ra.data % 1048576;
	sramifin_v.wd = 0;
	break;
      case STORE:
	v.rs[exec_i].common.state = RS_DONE;
	v.rs[exec_i].common.pc_next = t.pc + 1;
	sramifin_v.op = SRAM_STORE;
	sramifin_v.addr = t.rb.data % 1048576;
	sramifin_v.wd = t.ra.data;
	break;
      case IN:
	if(mem_in.recvifout.empty == 0){
	  v.rs[exec_i].common.state = RS_EXECUTING;
	  v.countdown[exec_i] = 0;//"000"
	  recvifin_v.rd_en = 1;
	}else
	  exec_complete = 0;//false
	break;
      case OUT:
	if(mem_in.transifout.full == 0){
	  v.rs[exec_i].common.state = RS_DONE;
	  v.rs[exec_i].common.pc_next = t.pc + 1;
	  transifin_v.wr_en = 1;
	  transifin_v.din = t.ra.data % 256;//7 downto 0
	}else
	  exec_complete = 0;//false
	break;
      }
    }
    if(exec_complete){
      v.rs_exec = m_r.rs_exec + 1;
      v.dummy_done = 0;
    }
  }
  mem_out.sramifin = sramifin_v;
  mem_out.recvifin = recvifin_v;
  mem_out.transifin = transifin_v;

  if(m_r.rs_full == 0 && mem_in.rs_in.op != NOP){
    v.rs[m_r.rs_youngest] = mem_in.rs_in;
    v.rs_youngest = m_r.rs_youngest + 1;
  }
  
  if(mem_in.cdb_next == 1 || m_r.cdb_out.tag.valid == 0){
    // clear rs
    if(m_r.cdb_out.tag.valid == 1){
      v.rs[m_r.rs_oldest] = mem_rs_zero;
      v.countdown[m_r.rs_oldest] = 0;
      v.rs_oldest = m_r.rs_oldest + 1;
    }
    v.cdb_out = cdb_zero;
    if(v.rs[v.rs_oldest].common.state == RS_DONE){
      t = v.rs[v.rs_oldest].common;
      v.cdb_out = make_cdb_out(t);
    }
  }
  if(v.rs_youngest+1 == v.rs_oldest)
    v.rs_full = 1;
  else
    v.rs_full = 0;

  //synchronous reset
  if(mem_in.rst == 1)
    m_r_in = m_reg_zero;
  else
    m_r_in = v;

  if(rst)
    m_r=m_reg_zero;
  else
    m_r=m_r_in;

  print_mem_rs(m_r);
  
  mem_out.rs_full = m_r.rs_full;
  mem_out.cdb_out = m_r.cdb_out;

  return mem_out;
}
