#include <stdio.h>
#include <stdint.h>
/*
  a,bはNaNではないとする。
  a>b...1
  a=b...0
  a<b...-1
  err...未定義
 */
int fcmp(uint32_t a, uint32_t b){
  int sgna = a >> 31;
  int sgnb = b >> 31;

  if(a == b)
    return 0;
  
  if(!a){//a=0
    if(sgnb)//b<0
      return 1;
    else
      return -1;
  }else if(!b){//b=0
    if(sgna)//a<0
      return -1;
    else
      return 1;
  }
  
  if(sgna != sgnb){
    if(sgna)//a<0,b>0
      return -1;
    else
      return 1;
  }

  
  if((a << 1) > (b << 1)){//|a| > |b|
    if(sgna)//a,b<0
      return -1;
    else
      return 1;
  }else if(sgna)
      return 1;

  return -1;
}
  
  
