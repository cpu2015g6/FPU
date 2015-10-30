#include <stdio.h>
#include <stdint.h>

extern uint32_t fmulprec(uint32_t a, uint32_t b);
extern int fcmp(uint32_t a, uint32_t b);
extern uint32_t finv(uint32_t a);
extern void print23bin(unsigned int a);
extern void print13bin(unsigned int a);
extern void printbinn(unsigned int a);

int main(){
  int i = 22;
  
  uint32_t a=127<<23;
  uint32_t b = a;
  
  while(a<(129<<23)){
    b= 127 << 23;
    i = 22;
    while(i>=0){
      b += 1<<i;
      if(fcmp(a,fmulprec(b,b)) < 0)
	b -= 1<<i;
      i--;
    }
    //printbinn(a);
    //print23bin(b);
    print13bin(finv(b)+1);
    a+=(1<<14);
  }
  return 0;
}
