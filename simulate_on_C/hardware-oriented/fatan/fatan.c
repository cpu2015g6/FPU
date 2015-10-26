#include <stdio.h>
#include <stdint.h>

extern int encode(uint32_t a);
extern uint32_t fmul(uint32_t a, uint32_t b);

uint32_t kernel_atan(uint32_t a){
  uint32_t x2,x3,x4,x5,x7,x9,x11,x13;
  x2=fmul(a,a);
  x3=fmul(a,x2);
  x4=fmul(x2,x2);
  x5=fmul(x2,x3);
  x7=fmul(x5,x2);
  x9=fmul(x5,x4);
  x11=fmul(x4,x7);
  x13=fmul(x4,x9);

  return fadd(fadd(fadd(a,fmul(0xbeaaaaaa,x3)),fadd(fmul(0x3e4ccccd,x5),fmul(0xbe124925,x7))),fadd(fadd(fmul(0x3de38e38,x9),fmul(0xbdb7d66e,x11)),fmul(0x3d75e7c5,x13)));
  
}  

uint32_t fatan(uint32_t a){
  int typa = encode(a);

  if(typa == 7)
    return 0xffc00000;//NaN

  
  
  return fmulsub(a,b);
}
  
