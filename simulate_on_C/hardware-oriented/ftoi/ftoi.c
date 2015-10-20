#include <stdio.h>
#include <stdint.h>

uint32_t ftoi(uint32_t f){
  
  uint32_t exp = (f << 1) >> 24;//exp<=f(30 downto 23)
  uint32_t ub = exp - 127;//ub<=exp - 126
  uint32_t man = (f << 9) >> 9;
  uint32_t absans;
  uint32_t sgn = f >> 31;
  uint32_t abs = (f << 1) >> 1;//abs<=f(30 downto 0)
  
  if(abs >= (158 << 23))//overflow
    return (sgn << 31) + 0x7fffffff;
  
  if(abs < (126 << 23))//underflow
    return 0;
  
  if(ub > 22)
    absans = (1 << ub) + (man << (ub - 23));//absans<=zero(downto) & "1" & man(22 downto 0)
  else
    absans = (1 << ub) + (man >> (23 - ub));
  
  if(!sgn)
    return absans;
  
  return (1 << 31) + ((1 << 31) - absans) - 1;
}
  
  
