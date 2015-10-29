#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern int fcmp(uint32_t a, uint32_t b);
extern void printtable(int *table);
extern void printbin(uint32_t a);
extern uint32_t fadd(uint32_t a, uint32_t b);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern uint32_t fdiv(uint32_t a, uint32_t b);
extern uint32_t finv(uint32_t a);
extern uint32_t finv_s(uint32_t a);
extern uint32_t fsqrt(uint32_t a);

uint32_t enc(char *p){
  int i=0;
  uint32_t o=0;
  while(i<32){
   o = o << 1;
   if(p[i] == '1')
     o++;
   i++;
  }
  return o;
}

void print_bin(uint32_t x) {
    int i;
    for (i = 31; i >= 0; --i) {
        printf("%d", (x >> i) & 1);
        if (i == 31 || i == 23) printf(" ");
    }
    printf("\n");
}

int main(int argc, char*argv[]){
  
  uint32_t i;
  uint32_t result;
  uint32_t out;
  int k=0;
  int del = 0;
  int table[128];

  i=0;
  while(i<128){
    table[i]=0;
    i++;
  }
  int offset = 1;
  i=(127<<23) + (offset << 13);

  while(k < 8192){
    result=finv_s(i);
    out = finv(i);
    del = out - result;
    if(del == -9){
      printbin(i);
      printbin(out);
      printbin(result);
      return 0;
     }
    table[del+64] = table[del+64]+1;
    k += 1;
    i += 1;
  }
  
  
  
  printtable(table);
  
  
  return 0;
}
