#include <stdio.h>
#include <stdint.h>

extern int encode(uint32_t a);

uint32_t mulroundp(uint32_t ulp, uint32_t f){
  if(f >> 31){
    if(f << 1)
      return 1;
    else
      return ulp;
  }
  return 0;
}

void abaaddsub(int *aba, int d){
  if(aba[d]){
    aba[d]=0;
    abaaddsub(aba,d+1);
  }else
    aba[d]=1;
}

void abaadd(int *aba, uint32_t b, int d){
  int i=0;
  while(i < 24){
    if((b >> i) % 2)
      abaaddsub(aba,i+d);
    i++;
  }
}

void abamul(int *aba, uint32_t a, uint32_t b){
  int i = 0;
  while(i<24){
    if((a >> i) % 2)
      abaadd(aba,b,i);
    i++;
  }
}

uint32_t fmulsubprec(uint32_t a, uint32_t b){
  uint32_t sgn = (a >> 31) ^ (b >> 31);
  uint32_t expa = (a >> 23) % 256;
  uint32_t expb = (b >> 23) % 256;
  int aba[48];
  uint32_t exp;
  uint32_t mana = (1<<23) + ((a << 9) >> 9);
  uint32_t manb = (1<<23) + ((b << 9) >> 9);
  uint32_t man=0;
  uint32_t frac=0;
  int i=0;
  while(i<48){
    aba[i]=0;
    i++;
  }
  
  abamul(aba,mana,manb);

  if(aba[47]){//繰り上がり
    exp = expa + expb - 126;
    i=46;
    while(i > 23){
      if(aba[i])
	man += 1 << (i - 24);
      i--;
    }
    while(i >= 0){
      if(aba[i])
	frac += 1 << (i + 8);
      i--;
    }
  }else{
    exp = expa + expb - 127;
    i=45;
    while(i > 22){
      if(aba[i])
	man += 1 << (i - 23);
      i--;
    }
    while(i >= 0){
      if(aba[i])
	frac += 1 << (i + 9);
      i--;
    }
  }
  if(exp > 400)
    return sgn << 31;
  if(exp >= 255)
    return (sgn << 31) + 0x7f800000;

  return (sgn << 31) + (exp << 23) + man + mulroundp((man%2),frac);
}
  
//-0.0の入る演算のみサポートしていない
uint32_t fmulprec(uint32_t a, uint32_t b){
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
  
  return fmulsubprec(a,b);
}
  
