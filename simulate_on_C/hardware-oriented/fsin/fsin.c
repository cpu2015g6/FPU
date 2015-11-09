#include <stdio.h>
#include <stdint.h>

extern uint32_t fadd(uint32_t a, uint32_t b);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern int fcmp(uint32_t a, uint32_t b);
extern uint32_t fneg(uint32_t a);

#define MYPI 0x40490fda
#define MYPI2 0x3fc90fda
#define MYPI4 0x3f490fda

uint32_t kernel_sin(uint32_t f){

  uint32_t x2 = fmul(f,f);
  uint32_t x3 = fmul(f,x2);
  uint32_t x4 = fmul(x2,x2);
  uint32_t x5 = fmul(x3,x2);
  uint32_t x7 = fmul(x3,x4);

  return fadd(fadd(f,fmul(0xb94d64b6,x7)), fadd(fmul(0xbe2aaaac,x3),fmul(0x3c088666,x5)));
}

uint32_t kernel_cos(uint32_t f){

  uint32_t x2 = fmul(f,f);
  uint32_t x3 = fmul(f,x2);
  uint32_t x4 = fmul(x2,x2);
  uint32_t x6 = fmul(x3,x3);

  return fadd(fadd(0x3f800000,fmul(0xbf000000,x2)), fadd(fmul(0x3d2aa789,x4),fmul(0xbab38106,x6)));
}

/*[0,pi/4)に簡約*/
uint32_t fsin(uint32_t f){

  if(fcmp(f,0) == 0)
    return fneg(fsin(f - (1 << 31)));
  if(fcmp(f,0x40c90fda) == 2)
    return fsin(fadd(f,0xc0c90fda));
  if(fcmp(f,MYPI) == 0){
    if(fcmp(f,MYPI2) == 0){
      if(fcmp(f,MYPI4) == 0)
	return kernel_sin(f);
      else
	return kernel_cos(fadd(MYPI2,fneg(f)));
    }else{
      if(fcmp(f,fadd(MYPI4,MYPI2)) == 0)
	kernel_cos(fadd(f,fneg(MYPI2)));
      else
	kernel_sin(fadd(MYPI,fneg(f)));
    }
  }else{
    f = fadd(f,fneg(MYPI));
    if(fcmp(f,MYPI2) == 0){
      if(fcmp(f,MYPI4) == 0)
	return fneg(kernel_sin(f));
      else
	return fneg(kernel_cos(fadd(MYPI2,fneg(f))));
    }else{
      if(fcmp(f,fadd(MYPI4,MYPI2)) == 0)
	fneg(kernel_cos(fadd(f,fneg(MYPI2))));
      else
	fneg(kernel_sin(fadd(MYPI,fneg(f))));
    }
  }
  
}
  
  
