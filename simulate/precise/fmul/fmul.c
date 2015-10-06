#include <stdio.h>
#include <stdint.h>

extern int encode(uint32_t a);

uint32_t mulround(uint32_t ulp, uint32_t f){
  if(f >> 31){
    if(f << 1)
      return 1;
    else
      return ulp;
  }
  return 0;
}

/*mantissaを3つに分け並列計算*/
uint32_t fmulsub(uint32_t a, uint32_t b){
  uint32_t sgn = (a >> 31) ^ (b >> 31);
  uint32_t expa = (a >> 23) % 256;
  uint32_t expb = (b >> 23) % 256;
  uint32_t exp;
  uint32_t ma1,ma2,ma3,mb1,mb2,mb3;
  uint32_t r11,r12,r13,r21,r22,r23,r31,r32,r33;
  uint32_t man;
  uint32_t frac;
  
  if(expa){
    ma1 = (1 << 7) + ((a << 9) >> 25);
    ma2 = ((a << 16) >> 24);
    ma3 = ((a << 24) >> 24);
  }else{
    ma1 = (a << 9) >> 24;
    ma2 = ((a << 17) >> 24);
    ma3 = ((a << 25) >> 24);
  }
  
  if(expb){
    mb1 = (1 << 7) + ((b << 9) >> 25);
    mb2 = ((b << 16) >> 24);
    mb3 = ((b << 24) >> 24);
  }else{
    mb1 = (b << 9) >> 24;
    mb2 = ((b << 17) >> 24);
    mb3 = ((b << 25) >> 24);
  }
  
  //software instruction
  r11 = ma1*mb1;
  r12 = ma1*mb2;
  r13 = ma1*mb3;
  r21 = ma2*mb1;
  r22 = ma2*mb2;
  r23 = ma2*mb3;
  r31 = ma3*mb1;
  r32 = ma3*mb2;
  r33 = ma3*mb3;
  
  if(((r11 << 8) + r12 + r21 + ((r22 + r13 + r31) >> 8)) >> 23){//繰り上がり
    man = (((r11 << 8) + r12 + r21 + ((r22 + r13 + r31) >> 8)) << 9) >> 9;//ゲタ履かせる
    exp = expa + expb - 126;
    frac = ((r33 + ((r23 + r32) << 8) + (((r22 + r13 + r31) % 256) << 16))) << 8;
  }else{
    man = ((r11 << 18) >> 9) + ((r12 + r21) << 1) + ((r22 + r13 + r31) >> 7) + ((r23 + r32) >> 16);
    exp = expa + expb - 127;
    frac = (r33 + ((r23 + r32) << 8) + (((r22 + r13 + r31) % 256) << 16)) << 9;
  }
  if(exp > 400)
    return 0;
  else if(exp >= 255)
    return (sgn << 31) + (255 << 23);
  
  return (sgn << 31) + (exp << 23) + man + mulround((man%2),frac);
}
  
//-0.0の入る演算のみサポートしていない
uint32_t fmul(uint32_t a, uint32_t b){
  int typa = encode(a);
  int typb = encode(b);

  if(typa == 7 || typb == 7)
    return 0xffc00000;//NaN
  if((typa == 4 || typa == 5) && typb == 6)
    return 0xffc00000;
  if((typb == 4 || typb == 5) && typa == 6)
    return 0xffc00000;
  if((typa == 4 && (typb % 2)) || (typb == 4 && (typa % 2)))
    return 0xff800000;//-Inf
  if((typa == 4 && !(typb % 2)) || (typb == 4 && !(typa % 2)))
    return 0x7f800000;//+Inf
  if((typa == 5 && (typb % 2)) || (typb == 5 && (typa % 2)))
    return 0x7f800000;
  if((typa == 4 && !(typb % 2)) || (typb == 4 && !(typa % 2)))
    return 0xff800000;
  if(typa > 1 && typb > 1)
    return 0;
  
  return fmulsub(a,b);
}
  
