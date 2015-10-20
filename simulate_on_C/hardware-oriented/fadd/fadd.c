#include <stdio.h>
#include <stdint.h>

extern int encode(uint32_t a);
extern int fcmp(uint32_t a, uint32_t b);

int round2(uint32_t ulp, uint32_t f){
  if(f >> 31){
    if(f << 1)
      return -1;
    else
      return -ulp;
  }
  
  return 0;
}

uint32_t priorityencoder(uint32_t r){//software instruction
  int i = 23;
  while(i >= 0){
    if(r >> i)
      break;
    i--;
  }
  return 23-i;
}

uint32_t revfadd(uint32_t a, uint32_t b){
  
  uint32_t sgn;
  uint32_t exp;
  uint32_t man;
  uint32_t expp,expi;
  uint32_t manp,mani;
  uint32_t result;
  uint32_t frac,prefrac;
  uint32_t del;
  uint32_t prior,infer;

  uint32_t down;
  
  if(fcmp(((a << 1) >> 1),((b << 1) >> 1)) > 0){
    prior=a;
    infer=b;
  }else{
    prior=b;
    infer=a;
  }
  sgn = prior >> 31;

  expp = (prior >> 23) % 256;
  expi = (infer >> 23) % 256;
  del = expp - expi;

  if(del > 24)
    return prior;

  if(expp)
    manp = (1 << 23) + ((prior << 9) >> 9);//ケチ表現をとる
  else
    manp = ((prior << 9) >> 9);//非正規化数(or 0),ケチ表現なし
  if(expi)
    mani = (1 << 23) + ((infer << 9) >> 9);
  else
    mani = ((infer << 9) >> 9);
  
  if(expp && expi){
    result = manp - (mani >> del);
    prefrac = mani << (32 - del);
  }else if(expp){
    result = manp - (mani >> (del-1));
    prefrac = mani << (32 - del);
  }else{
    result = manp - mani;
    prefrac = 0;
  }
  
  if(expp)
    down = priorityencoder(result);
  else
    down = 0;
  
  if(down == 24)
    return 0;
  
  if(expp - down > 0){//結果が正規化数
    exp = expp - down;
    frac = prefrac << down;
    man = (((result << (9 + down)) >> 9) - ((prefrac >> 1) >> (31 - down)));
  }else{
    exp = 0;
    frac = 0;
    man = (result << (expp - 1)) - ((prefrac >> 1) >> (31 - del));
  }
  
  return (sgn << 31) + (exp << 23) + man + round2((man % 2),frac);
}

int round1(uint32_t ulp, uint32_t f){
  if(f >> 31){
    if(f << 1)
      return 1;
    else
      return ulp;
  }
  return 0;
}

uint32_t absfadd(uint32_t a, uint32_t b){
  
  uint32_t sgn = a >> 31;
  uint32_t exp;
  uint32_t man;
  uint32_t expp,expi;
  uint32_t manp,mani;
  uint32_t result;
  uint32_t frac,prefrac;
  uint32_t del;
  uint32_t prior,infer;
  
  if(fcmp(((a << 1) >> 1),((b << 1) >> 1)) > 0){//|a| > |b|
    prior=a;
    infer=b;
  }else{
    prior=b;
    infer=a;
  }

  expp = (prior >> 23) % 256;
  expi = (infer >> 23) % 256;
  del = expp - expi;

  if(del > 24)
    return prior;

  if(expp)
    manp = (1 << 23) + ((prior << 9) >> 9);//ケチ表現をとる
  else
    manp = ((prior << 9) >> 9);//非正規化数(or 0),ケチ表現なし
  if(expi)
    mani = (1 << 23) + ((infer << 9) >> 9);
  else
    mani = ((infer << 9) >> 9);

  if(expp && expi){
    result = manp + (mani >> del);
    prefrac = mani << (32 - del);
  }else if(expp){
    result = manp + (mani >> (del-1));
    prefrac = mani << (32 - del);
  }else{
    result = manp + mani;
    prefrac = 0;
  }

  if(result >> 24){//繰り上がり
    exp = expp + 1;
    frac = ((result % 2) << 31) + (prefrac >> 1);
    man = (result << 8) >> 9;
  }else if(result >> 23){//繰り上がりなしor非正規->正規
    exp = expp;
    frac = prefrac;
    man = (result << 9) >> 9;
    if(!(expp + expi))
      exp++;
  }else{//非正規数
    exp = 0;
    frac = 0;
    man = result;
  }
  if(exp == 255)//ovarflow
    return (sgn << 31) + 0x7f800000;
  
  return (sgn << 31) + (exp << 23) + man + round1((man % 2),frac);
}

uint32_t fadd(uint32_t a, uint32_t b){

  int typa = encode(a);
  int typb = encode(b);
  /*
    Encode float
    NaN...111 (7)
    +Inf...100 (4)
    -Inf...101 (5)
    0...110 (6)
    +Denormalized Number...010 (2)
    -Denormalized Number...011 (3)
    +Normalized Number...000 (0)
    -Normalized Number...001 (1)
   */
  if(typa == 7 || typb == 7)
    return 0xffc00000;//NaN
  else if((typa == 4 && typb == 5) || (typa == 5 && typb == 4))
    return 0xffc00000;//NaN
  else if(typa == 4 || typb == 4)
    return 0x7f800000;//+Inf
  else if(typa == 5 || typb == 5)
    return 0xff800000;//-Inf
  else if(typa == 6)
    return b;//a=0
  else if(typb == 6)
  return a;//b=0
  else if((typa % 2) ^ (typb % 2))
    return revfadd(a,b);
 
  return absfadd(a,b);
}
