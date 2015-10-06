#include <stdio.h>
#include <stdint.h>

extern int fcmp(uint32_t a, uint32_t b);

void print_bina(uint32_t x) {
    int i;
    for (i = 31; i >= 0; --i) {
        printf("%d", (x >> i) & 1);
        if (i == 31 || i == 23) printf(" ");
    }
    printf("\n");
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
  
