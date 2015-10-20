#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*2分法で求める*/
uint32_t ftoi_s(uint32_t f){
  
  float fl;
  uint32_t sgn = f >> 31;

  memcpy(&fl,&f,4);
  
  int i = 0;
  uint32_t ans;

  int j = 30;
  
  if(!sgn){//f:pos
    while(j >= 0){
      i += 1 << j;
      if(fl < i)
	i -= 1 << j;
      j--;
    }//fを超えない最大の値が入る
    if(i + 1 - f < f - i)
      i += 1;
  }else{
    while(j >= 0){
      i -= 1 << j;
      if(fl > i)
	i += 1 << j;
      j--;
    }//fを下回らない最小の値が入る
    if(f + 1 - i < i - f)
      i -= 1;
  }
  memcpy(&ans,&i,4);
  i = (int)fl;
  memcpy(&ans,&i,4);
  return ans;
}
  
