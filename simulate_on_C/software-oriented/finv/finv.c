#include <stdio.h>
#include <stdint.h>


extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(uint32_t x);
extern int encode(uint32_t a);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern int pencoder(uint32_t a);

uint32_t invsub(uint32_t a){

  uint32_t sgn = a >> 31;
  uint32_t expa = (a >> 23) % 256;
  uint32_t exp = 254 - expa - 1;
  uint32_t ans = (sgn << 31) + (exp << 23);

  
  if(!(a << 9))//aのman=0
    return (sgn << 31) + ((exp + 1) << 23);

  if(expa == 254)
    return (sgn << 31) + 0;

  //software instruction
  int i = 22;
  while(i >= 0){
    ans += (1 << i);
    if(fcmp((127 << 23),fmul(a,ans)) < 0)
      ans -= (1 << i);
    i--;
  }
  return ans;
}
  
  

uint32_t finv(uint32_t a){
  
  int typa = encode(a);
  int nega=0;

  if(typa == 6 && (a >> 31))//-0の拡張
    nega=1;

  if(typa == 7)
    return 0xffc00000;
  if(nega)// 1 / -0
    return 0xff800000;
  if(typa == 6)
    return 0x7f800000;
  if(typa == 4)// 1 / +Inf
    return 0;
  if(typa == 5)// 1 / -Inf
    return 0x80000000;
  if(typa > 1)
    return ((a >> 31) << 31) + 0x7f800000;
  
  return invsub(a);
}
  
