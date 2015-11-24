#include "common.h"

uint32_t sram[1048576];//4MB
uint32_t addrhist[2];
int xwahist[2];

uint32_t srambeh(int xwa,uint32_t addr,uint32_t din){//latency 2
  uint32_t ans;
  if(xwahist[1])
    ans = sram[addrhist[1]];
  else{
    fprintf(stderr,"-----------sram write------------\n");
    fprintf(stderr,"addr: %d\n",addrhist[1]);
    fprintf(stderr,"data: %d\n",din);
    fprintf(stderr,"---------------------------------\n");
    sram[addrhist[1]] = din;
    ans = din;
  }
  addrhist[1]=addrhist[0];
  xwahist[1]=xwahist[0];
  addrhist[0]=addr;
  xwahist[0]=xwa;
  return ans;
}

typedef struct{
  sramif_op op;
  uint32_t data;
}cmd_type;

cmd_type cmd_zero = {SRAM_NOP,0};
#define CMD_ZERO {SRAM_NOP,0}

typedef struct{
  int out_enable;//std_logic
  int xstore;//std_logic
  int nop;//std_logic
  uint32_t store_data;
  cmd_type hist[4];
  sramif_in sramifin;
  sramif_out sramifout;
}sram_reg_type;

sram_reg_type sram_reg_zero = {0,0,0,0,{CMD_ZERO,CMD_ZERO,CMD_ZERO,CMD_ZERO},SRAMIF_IN_ZERO,SRAMIF_OUT_ZERO};
#define SRAM_REG_ZERO {0,0,0,0,{CMD_ZERO,CMD_ZERO,CMD_ZERO,CMD_ZERO},SRAMIF_IN_ZERO,SRAMIF_OUT_ZERO}

sram_reg_type s_r = SRAM_REG_ZERO;
sram_reg_type s_r_in = SRAM_REG_ZERO;

sramif_out sramif(int clk, int rst, sramif_in sramifin){
  sram_reg_type v;
  cmd_type nextcmd;
  sramif_out sramifout;
  int XWA;
  if(s_r.sramifin.op == SRAM_STORE)
    XWA=0;
  else
    XWA=1;
   
  uint32_t ZDin;
  if(s_r.xstore == 0)
    ZDin = s_r.store_data;
  else
    ZDin = 0;
    
  uint32_t ZDout = srambeh(XWA,s_r.sramifin.addr,ZDin);//not treat nop
  
  v = s_r;
  v.sramifin = sramifin;
  v.hist[1] = s_r.hist[0];
  v.hist[2] = s_r.hist[1];
  v.hist[3] = s_r.hist[2];
  v.hist[0].op = s_r.sramifin.op;
  v.hist[0].data = s_r.sramifin.wd;
  v.sramifout.data_enable = s_r.out_enable;
  if(s_r.out_enable)
    v.sramifout.rd = ZDout;
  else
    v.sramifout.rd = 0;
  nextcmd = s_r.hist[0];
  
  switch(nextcmd.op){
  case SRAM_NOP:
    v.nop = 1;
    v.out_enable = 0;
    v.xstore = 1;
    break;
  case SRAM_LOAD:
    v.nop = 0;
    v.out_enable = 1;
    v.xstore = 1;
    break;
  case SRAM_STORE:
    v.nop = 0;
    v.out_enable = 0;
    v.xstore = 0;
    v.store_data = nextcmd.data;
    break;
  }
  s_r_in = v;

  if(rst)
    s_r = sram_reg_zero;
  else
    s_r = s_r_in;  

  return sramifout;
}
  
  
