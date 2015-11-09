#include <stdint.h>

#define MYPI2 0x3fc90fda
#define MYPI4 0x3f490fda

extern int fcmp(uint32_t a,uint32_t b);
extern uint32_t fneg(uint32_t a);
extern uint32_t fadd(uint32_t a, uint32_t b);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern uint32_t finv(uint32_t a);

uint32_t kernel_atan(uint32_t a){
  uint32_t a2 = fmul(a,a);
  uint32_t a3 = fmul(a,a2);
  uint32_t a5 = fmul(a3,a2);
  uint32_t a7 = fmul(a5,a2);
  uint32_t a9 = fmul(a7,a2);
  uint32_t a11 = fmul(a9,a2);
  uint32_t a13 = fmul(a11,a2);
  
  return fadd(a,fadd(fmul(0xbeaaaaaa,a3),fadd(fmul(0x3e4ccccd,a5),fadd(fmul(0xbe124925,a7),fadd(fmul(0x3de38e38,a9),fadd(fmul(0xbdb7d66e,a11),fmul(0x3d75e7c5,a13)))))));
}

uint32_t fatan(uint32_t a){

  if(fcmp(a,0) == 2){
    if(fcmp(a,0x3ee00000) == 0)
      return kernel_atan(a);
    else if(fcmp(a,0x401c0000) == 0)
      return fadd(MYPI4,kernel_atan(fmul(fadd(a,0xbf800000),finv(fadd(a,0x3f800000)))));
    else
      return fadd(MYPI2,fneg(kernel_atan(fmul(0x3f800000,finv(a)))));
  }
  
  return fneg(fatan(fneg(a)));
}
