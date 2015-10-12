#include <stdio.h>
#include <stdint.h>
#include "sqrtrom.h"

extern void printbin(uint32_t x);
extern int encode(uint32_t a);
extern uint32_t ctou(char *c);

uint32_t fmul_sqrt(uint32_t data2, uint32_t del, int flag){
  uint32_t d1,d2,d3,de1,de2,de3;
  uint32_t r11,r12,r21,r22,r13,r31;
  uint32_t ans;
  d1=data2>>19;
  d2=(data2>>15)%16;
  d3=(data2>>11)%16;
  de1=del>>8;
  de2=(del>>4)%16;
  de3=del%16;
  
  r11=d1*de1;
  r12=d1*de2;
  r21=d2*de1;
  r22=d2*de2;
  r13=d1*de3;
  r31=d3*de1;
  
  ans=del + (r11 << 4) + (r12 + r21) + ((r22 + r13 + r31) >> 4);
  if(flag)
    return ans >> 1;
  return ans >> 2;
}
  

uint32_t fsqrt(uint32_t a){
 
  int typa = encode(a);
  uint32_t expa = (a >> 23) % 256;
  uint32_t exp = ((expa - 127) / 2) + 127;
  uint32_t addr = (a << 8) >> 21;//addr<=a(23 downto 13)
  uint32_t data = ctou(sqrtrom[addr]);//data<=rom(conv_integer(addr))
  uint32_t data2 = ctou(sqrtrom2[addr]);//data2<=rom2(conv_integer(addr))
  uint32_t del = a % 8192;//del<=a(12 downto 0)
  if(addr == 1024)
    del *= 2; 


  if(typa == 7)
    return 0xffc00000;
  if(typa == 6)
    return 0;
  if(typa == 4)
    return 0x7f800000;
  if(addr>>10)//繰り下がりがあるかどうか
    return (exp << 23) + data + fmul_sqrt(data2,del,0);

  return (exp << 23) + data + fmul_sqrt(data2,del,1);
  	
}
