#include <stdio.h>
#include <stdint.h>

extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(uint32_t x);
extern int encode(uint32_t a);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern int pencoder(uint32_t a);

uint32_t fdivsub(uint32_t a, uint32_t b){
  uint32_t sgn = (a >> 31) ^ (b >> 31);
  uint32_t mana,manb;
  uint32_t expa,expb;
  uint32_t exp;
  if(!((a >> 23) % 256)){
    expa = 7 - pencoder(a << 1);
    mana = (a << (1 + pencoder(a << 1))) >> 8;
  }else{
    expa = (a >> 23) % 256;
    mana = (1 << 23) + ((a << 9) >> 9);
  }
  
  if(!((b >> 23) % 256)){
    expb = 7 - pencoder(b << 1);
    manb = (b << (1 + pencoder(b << 1))) >> 8;
  }else{
    expb = (b >> 23) % 256;
    manb = (1 << 23) + ((b << 9) >> 9);
  }

  if(fcmp(mana,manb) >= 0)
    exp = expa - expb + 127;
  else
    exp = expa - expb + 126;

  if(exp > 400)
    return sgn << 31;//+-0
  
  if(exp >= 255)
    return (sgn << 31) + 0x7f800000;//+-Inf
  
  //software instruction
  int i=22;
  uint32_t opr;
  opr = exp << 23; 
  while(i >= 0){
    opr += 1 << i;
    if(fcmp((a << 1) >> 1,fmul(opr,(b << 1) >> 1)) < 0)
      opr -= 1 << i;
    i--;
  }
  return (sgn << 31) + opr;
}
  
uint32_t fdiv(uint32_t a, uint32_t b){
  
  int typa = encode(a);
  int typb = encode(b);
  int nega=0;
  int negb=0;

  if(typa == 6 && (a >> 31))//-0の拡張
    nega=1;
  if(typb == 6 && (b >> 31))
    negb=1;

  if(typa == 7 || typb == 7)
    return 0xffc00000;
  if(typa * typb == 20 || typa * typb == 16 || typa * typb == 25)//Inf / Inf
    return 0xffc00000;
  if(typa == 6 && typb == 6)// 0 / 0
    return 0xffc00000;
  if(typa == 6 && nega){// -0 /
    if(typb % 2)// -0 / -hoge
      return 0;//+0
    else
      return 0x80000000;//-0
  }else if(typa == 6){// +0 /
    if(typb % 2)
      return 0x80000000;//-0
    else
      return 0;//+0
  }
  if(typb == 6 && negb){//  / -0
    if(typa % 2) // -hoge / -0
      return 0x7f800000;//+Inf
    else
      return 0xff800000;//-Inf
  }else if(typb == 6){//  / +0
    if(typa % 2)// -hoge / +0
      return 0xff800000;//-Inf
    else
      return 0x7f800000;//+Inf
  }
  if(typa == 4 || typa == 5){// Inf /
    if((typa % 2) ^ (typb % 2))
      return 0xff800000;//-Inf
    else
      return 0x7f800000;//+Inf
  }
  if(typb == 4 || typb == 5){//  / Inf
    if((typa % 2) ^ (typb % 2))
      return 0x80000000;//-0
    else
      return 0;//+0
  }

  return fdivsub(a,b);
}
