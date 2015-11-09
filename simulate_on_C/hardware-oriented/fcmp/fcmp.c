#include <stdio.h>
#include <stdint.h>
/*
  a,bはNaNではないとする。
  a>b...1
  a=b...0
  a<b...-1
  err...未定義
 */

//it has no different between hard and soft
uint32_t fcmp(uint32_t a, uint32_t b){
  int sgna = a >> 31;//sgna<=a(31)
  int sgnb = b >> 31;//sgnb<=b(31)

  if(sgna != sgnb && !sgna)
    return 2;
  else if(sgna != sgnb && sgna)
    return 0;
  else if(a > b && !sgna)
    return 2;
  else if(b > a && sgna)
    return 2;
  else if(a != b)
    return 0;

  return 1;
}
  
  
