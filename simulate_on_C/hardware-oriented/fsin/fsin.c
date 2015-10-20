#include <stdio.h>
#include <stdint.h>

extern uint32_t fadd(uint32_t a, uint32_t b);
extern uint32_t fmul(uint32_t a, uint32_t b);

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
  
  uint32_t exp = ((f << 1) >> 24);
  uint32_t abs = (f << 1) >> 1;
  uint32_t flag = f >> 31;
  //uint32_t man = (1 << 23) + ((f << 9) >> 9);
  
  if(exp > 24 + 127)//f * 2^-23がpiより大きいとき、返す値は適当(基準)
    return 0;
  
  if(exp < 126)//[0,pi/4)
    return (flag << 31) + kernel_sin(abs);
  
  /*if(exp < 6 + 127){
    t4 = man >> ();
    man = man - pirom2[()];
    }*/
  
  if(abs >= MYPI){
    abs = fadd(((1 << 31) + MYPI),abs);
    flag = 1 - flag;
  }
  
  if(abs >= MYPI2){
    abs = fadd(MYPI,((1 << 31) + abs));
  }

  if(abs > MYPI4){
    abs = fadd(MYPI2,((1 << 31) + abs));
    return (flag << 31) + kernel_cos(abs);
  }

  return (flag << 31) + kernel_sin(abs);
}
  
  
