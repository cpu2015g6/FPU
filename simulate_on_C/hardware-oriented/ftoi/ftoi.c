#include <stdio.h>
#include <stdint.h>

uint32_t ftoi(uint32_t f){
  
  uint32_t exp = (f << 1) >> 24;//exp<=f(30 downto 23)
  uint32_t ub = exp - 127;//ub<=exp - 126
  uint32_t man = (f << 9) >> 9;
  uint32_t absans;
  uint32_t sgn = f >> 31;
  
  if(exp >= 31 + 127)//overflow
    return (sgn << 31) + 0x7fffffff;
  
  if(exp < 127){//underflow
    if(exp == 126 && sgn)
      return 0xffffffff;
    else if(exp == 126)
      return 1;
    return 0;
   }
  
  if(ub > 22)
    absans = (1 << ub) + (man << (ub - 23));//absans<=zero(downto) & "1" & man(22 downto 0)
  else
    absans = (1 << ub) + (man >> (23 - ub));
  
  if(!sgn)
    return absans;
  
  return (1 << 31) + ((1 << 31) - absans);
}
  
  
