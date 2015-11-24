#include "common.h"

#define T 1000

recvif_out_type rcv;

recvif_out_type recvif(int rn){

  return rcv;
}

void transmitter(int en, uint32_t data){
  //char c = data % 256;
  if(en)
    fprintf(stdout,"%d\n",data);
}

int main(int argc, char* argv[]){
  
  int clk=0;
  int rst=0;
  cpu_top_in_type cpu_top_in;
  cpu_top_out_type cpu_top_out;

  if(argc < 2){
    fprintf(stderr,"few args!!\n");
    return 1;
  }

  program_loader(argv[1]);//instant load

  while(clk < T){
    cpu_top_in.sramifout = sramif(clk, rst, cpu_top_out.sramifin);
    cpu_top_in.recvifout = recvif(cpu_top_out.recvifin.rd_en);
    cpu_top_out = cpu_top(clk,rst,cpu_top_in);
    transmitter(cpu_top_out.transifin.wr_en,cpu_top_out.transifin.din);
    clk++;
  }
  fprintf(stderr,"timeover %d clks \n",T);
  
  return 0;
}
  
