#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*cast*/
uint32_t ftoi_s(uint32_t f){
  
  float fl;
  memcpy(&fl,&f,4);
  
  int i;
  uint32_t ans;

  i = (int)fl;
  memcpy(&ans,&i,4);
  return ans;
}
  
