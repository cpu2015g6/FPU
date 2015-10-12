#include <stdio.h>
#include <stdint.h>

extern uint32_t fmulprec(uint32_t a, uint32_t b);
extern int fcmp(uint32_t a, uint32_t b);
extern uint32_t finv(uint32_t a);
extern void printbin(unsigned int a);
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
    //printbin(b);
    printbin(finv(b));
    a+=(1<<13);
  }
  return 0;
}
