#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t finv(uint32_t a, int offset);
extern uint32_t finv_s(uint32_t a);
extern void printtable(int *a);

int main(int argc, char*argv[]){
  uint32_t i,j;
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
  //j=9;
  //while(j<1024){
  j=0;
  k=0;
  i = (127 << 23) + (j << 13);
  while(k < 8192*1024){
    result=finv_s(i);
    out = finv(i,0);
    del = out - result;
    table[del+64] = table[del+64]+1;
    k++;
    i++;
  }
  /*if(table[69]){
   i=0;
  while(i<128){
    table[i]=0;
    i++;
  }
    k=0;
  i = (127 << 23) + (j << 13);
  while(k < 8192){
    result=finv_s(i);
    out = finv(i,0);
    del = out - result;
    table[del+64] = table[del+64]+1;
    k++;
    i++;
  }
   printf("offset=%d\n",j);
   printtable(table);
   return 1;
   }
  j++; 
  }*/
  printtable(table);
  return 0;

}
