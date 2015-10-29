#include <stdio.h>
#include <stdint.h>
#include "invrom.h"

;extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(uint32_t x);
//extern int encode(uint32_t a);
//extern int pencoder(uint32_t a);
extern uint32_t ctou23(char *c);
extern uint32_t vector(uint32_t a, int i, int j);




uint32_t fmul_square_man(uint32_t data){//23桁*23桁なのだが、上位14桁が出れば十分

  uint32_t c1 = (1 << 6) + vector(data,22,17);
  uint32_t c2 = vector(data,16,10);

  uint32_t ans = c1*c1 + (2*c1*c2 >> 7);
  
  return ans;
}
  
uint32_t fmul_man(uint32_t data2,uint32_t del){
  uint32_t c1 = vector(data2,13,7);
  uint32_t c2 = vector(data2,6,0);
  uint32_t d1 = vector(del,13,7);
  uint32_t d2 = vector(del,6,0);
  
  uint32_t ans = (c1*d1 + (c2*d1 >> 7) + (d2*c1 >> 7)) << 1;

  return ans;
}

uint32_t finv(uint32_t a){
 
  uint32_t sgn = vector(a,31,31);
  uint32_t exp = 253 - vector(a,30,23);
  uint32_t addr = vector(a,22,13);
  uint32_t del = vector(a,12,0);
  
  uint32_t data = ctou23(rom[addr]);//after 1clk

  uint32_t data2 = fmul_square_man(data);//after 2clk

  uint32_t del2 = fmul_man(data2,del);//after 3clk

  uint32_t ans = (sgn << 31) + (exp << 23) + data - del2;//after 4clk
  if(del >> 12)
    ans += 3;
  if(!(addr >> 9))
    ans += 3;  

  return ans;
  
}
  
