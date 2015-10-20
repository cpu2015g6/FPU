#include <stdio.h>
#include <stdint.h>

extern int pencoder(uint32_t a);

uint32_t itof(uint32_t i){
  
  uint32_t sgn = i >> 31;
  uint32_t abs;
  if(sgn)
    abs= (1 << 31) - ((i << 1) >> 1) + 1;
  uint32_t ub = pencoder(i << 1);
  uint32_t 
  
