#include <stdio.h>
#include <stdint.h>

extern uint32_t vector(uint32_t a,int i,int j);

uint32_t fmul(uint32_t a, uint32_t b){
  uint32_t sgn;
  uint32_t exp;
  uint32_t t11,t12,t13,t21,t22,t23;
  uint32_t r11,r12,r13,r21,r22,r23,r31,r32;
  uint32_t result;
  int zflag=0;

  if(!(vector(a,30,23)) || !(vector(b,30,23)))
    zflag=1;
  
  t11 = (1 << 7) + vector(a,22,16);
  t12 = vector(a,15,8);
  t13 = vector(a,7,0);
  t21 = (1 << 7) + vector(b,22,16);
  t22 = vector(b,15,8);
  t23 = vector(b,7,0);

  sgn = (a >> 31) ^ (b >> 31);
  exp = vector(a,30,23) + vector(b,30,23) - 127;

  r11 = t11*t21; 
  r12 = t11*t22;
  r21 = t12*t21;
  r22 = t12*t22;
  r31 = t13*t21;
  r13 = t11*t23;
  r23 = t12*t23;
  r32 = t13*t22;

  result = (r11 << 10) + (r12 << 2) + (r21 << 2) + 
           vector(r22,15,6) + vector(r13,15,6) + vector(r31,15,6) +
           vector(r23,15,14) + vector(r32,15,14);

  if(zflag)
   return 0;
  else if(vector(result,25,25))
   return (sgn << 31) + ((exp+1) << 23) + vector(result,24,2);
  return (sgn << 31) + (exp << 23) + vector(result,23,1);
}
  
