#include <stdio.h>
#include <stdint.h>

extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(uint32_t x);
extern int encode(uint32_t a);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern int pencoder(uint32_t a);

uint32_t fsqrt(uint32_t a){
  
  int typa = encode(a);
  uint32_t expa = (a >> 23) % 256;
  uint32_t exp = ((expa - 127) / 2) + 127;
  
  if(typa == 7)
    return 0xffc00000;
  if(typa == 6)
    return 0;
  if(typa == 4)
    return 0x7f800000;
  
  //software instruction
  int i = 22;
  uint32_t opr = exp << 23;
  while(i >= 0){
    opr += 1 << i;
    if(fcmp(a,fmul(opr,opr)) < 0)
      opr -= 1 << i;
    i--;
  }
  return opr;
}
