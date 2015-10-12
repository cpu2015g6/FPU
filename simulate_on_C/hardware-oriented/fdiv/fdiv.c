#include <stdio.h>
#include <stdint.h>

extern uint32_t fmul(uint32_t a, uint32_t b);
extern uint32_t finv(uint32_t a);

uint32_t fdiv(uint32_t a, uint32_t b){
  
  return fmul(a,finv(b));
}
