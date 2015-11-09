#include <stdio.h>
#include <stdint.h>
#include "invrom.h"


extern uint32_t ctou23(char *c);
extern uint32_t ctou13(char *c);
extern uint32_t vector(uint32_t a, int i, int j);
  
uint32_t fmul_man(uint32_t data2,uint32_t del){
  uint32_t c1 = vector(data2,13,7);
  uint32_t c2 = vector(data2,6,0);
  uint32_t d1 = vector(del,13,7);
  uint32_t d2 = vector(del,6,0);
  
  uint32_t ans = (c1*d1 + (c2*d1 >> 7) + (d2*c1 >> 7)) << 1;

  return ans;
}

uint32_t finv(uint32_t a,int offset){
 
  uint32_t sgn = vector(a,31,31);
  uint32_t exp = 253 - vector(a,30,23);
  uint32_t addr = vector(a,22,13);
  uint32_t del = vector(a,12,0);
  
  uint32_t data = ctou23(rom[addr]);//after 1clk
  uint32_t data2 = ctou13(rom2[addr]) + (1 << 13) + offset;//after 1clk
  if(addr >= 425)
   data2 /= 2;
 
  uint32_t del2 = fmul_man(data2,del);//after 2clk

  uint32_t ans = (sgn << 31) + (exp << 23) + data - del2;//after 3clk
  
  return ans;
  
}
  
