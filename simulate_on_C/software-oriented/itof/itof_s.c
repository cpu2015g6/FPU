#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*cast*/
uint32_t itof_s(uint32_t i){
  
  float fl;
  int in; 
  uint32_t ans;

  memcpy(&in,&i,4);

  fl = (float)in;

  memcpy(&ans,&fl,4);

  return ans;

}
  
