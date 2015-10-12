#include <stdio.h>
#include <stdint.h>

extern uint32_t absfadd(uint32_t a, uint32_t b);
extern uint32_t revfadd(uint32_t a, uint32_t b);
extern int encode(uint32_t a);

uint32_t fadd(uint32_t a, uint32_t b){

  int typa = encode(a);
  int typb = encode(b);
  /*
    Encode float
    NaN...111 (7)
    +Inf...100 (4)
    -Inf...101 (5)
    0...110 (6)
    +Denormalized Number...010 (2)
    -Denormalized Number...011 (3)
    +Normalized Number...000 (0)
    -Normalized Number...001 (1)
   */
  if(typa == 7 || typb == 7)
    return 0xffc00000;//NaN
  else if((typa == 4 && typb == 5) || (typa == 5 && typb == 4))
    return 0xffc00000;//NaN
  else if(typa == 4 || typb == 4)
    return 0x7f800000;//+Inf
  else if(typa == 5 || typb == 5)
    return 0xff800000;//-Inf
  else if(typa == 6)
    return b;//a=0
  else if(typb == 6)
  return a;//b=0
  else if((typa % 2) ^ (typb % 2))
    return revfadd(a,b);
 
  return absfadd(a,b);
}
