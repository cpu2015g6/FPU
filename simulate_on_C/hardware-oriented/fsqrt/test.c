#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern int fcmp(uint32_t a, uint32_t b);
extern void printtable(int *table);
extern uint32_t fsqrt_s(uint32_t a);
extern uint32_t fsqrt(uint32_t a);
extern void printbinn(uint32_t a);
extern void printbin(uint32_t a);

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

  if(argc < 2){
   printf("few args!!\n");
   return 1;
  }

  i=0;
  while(i<128){
    table[i]=0;
    i++;
  }
  int offset = atoi(argv[1]);
  i=(127<<23) + (offset << 14);
 
  //int dist = 0;
  //int j=-20;
  //while(j < 20){
    while(k < 16384 * 1024){
      result=fsqrt_s(i);
      out = fsqrt(i);
      del = out - result;
      /*if(del == 6){
	printbin(i);
	printbin(out);
	printbin(result);
	return 0;
	}*/
      table[del+64] = table[del+64]+1;
      k += 1;
      i += 1;
    }
    //}
  
  
  printtable(table);
  
  
  return 0;
}
