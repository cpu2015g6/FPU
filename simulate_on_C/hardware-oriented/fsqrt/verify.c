#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t fsqrt(uint32_t a, int offset);
extern uint32_t fsqrt_s(uint32_t a);
extern void printtable(int *a);
extern void printbin(uint32_t a);

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

  k=0;
  i = (127 << 23) + (0 << 14);
  while(k < 16384*1024){
    result=fsqrt_s(i);
    out = fsqrt(i,0);
    del = out - result;
    table[del+64] = table[del+64]+1;
    k++;
    i++;
  }
      
  printtable(table);
  return 0;

}
