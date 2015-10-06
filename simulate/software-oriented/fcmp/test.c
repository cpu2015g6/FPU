#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern int fcmp(uint32_t a, uint32_t b);
extern uint32_t fadd(uint32_t a, uint32_t b);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern uint32_t fdiv(uint32_t a, uint32_t b);
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

  uint32_t i = enc("11111101010010010100110010111110");
  uint32_t j = enc("01111001000111011100100001010111");
  int num = atoi(argv[1]);
  int k=0;
  float f1,f2;
  int err = 0;
  
  srand((unsigned) time(NULL));

  while(k < num){
    i = rand();
    j = rand();
    i += (rand() % 2) << 31;
    j += (rand() % 2) << 31;
    memcpy(&f1,&i,4);
    memcpy(&f2,&j,4);
    if( (((f1 < f2) && !(fcmp(i,j) < 0)) || (!(f1 < f2) && (fcmp(i,j) < 0))) 
	&& (((i >> 23) % 256) != 255) && (((j >> 23) % 256) != 255)){
      printf("miss!!\n");
      print_bin(i);
      print_bin(j);
      printf("output: %d\n\n",fcmp(i,j));
      err++;
    }
    k++;
  }
  printf("total: %d errors\n",err);
  
  return 0;
}
