#include <stdio.h>
#include <stdint.h>

extern uint32_t fmul_s(uint32_t a, uint32_t b);
extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(unsigned int a);
extern void printbinn(unsigned int a);
extern void print23bin(unsigned int a);

int main(){
  int i = 22;
  
  uint32_t a=127<<23;
  uint32_t b = a;
  uint32_t const1 = 127<<23; 
  
  while(a<(1<<30)){
    b= 126 << 23;
    i = 22;
    while(i>=0){
      b += 1<<i;
      if(fcmp(const1,fmul_s(a,b)) < 0)
	b -= 1<<i;
      i--;
    }
    //printbinn(a);
    print23bin(b);
    //print23bin(fmul_s(b,b));
    a+=(1<<13);
  }
  return 0;
}
