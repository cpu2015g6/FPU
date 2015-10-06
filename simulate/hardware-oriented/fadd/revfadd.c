#include <stdio.h>
#include <stdint.h>

extern int fcmp(uint32_t a, uint32_t b);

void print_binr(uint32_t x) {
    int i;
    for (i = 31; i >= 0; --i) {
        printf("%d", (x >> i) & 1);
        if (i == 31 || i == 23) printf(" ");
    }
    printf("\n");
}


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
