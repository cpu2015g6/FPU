#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern int fcmp(uint32_t a, uint32_t b);
extern uint32_t fcos(uint32_t a);
extern void printbinn(uint32_t a);

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
  int num = atoi(argv[1]);
  int k=0;
  double d1,dr;
  float f1,fr;
  int err = 0;
  
  srand((unsigned) time(NULL));

  while(k < num){
    i = rand();
    memcpy(&f1,&i,4);
    d1 = (double)f1;
    dr = cosf(d1);
    fr = (float)dr;
    memcpy(&result,&fr,4);
    out = fcos(i);
    if( fcmp(0,i) < 0 && fcmp(0x3fc90fda,i) > 0//オペランドは0 ~ 2pi
        && (fcmp(out,(result + 10)) == (fcmp(out,(result - 10))))
	&& (((out >> 23) % 256) + ((result >> 23) % 256))//非正規化数の丸め
	&& (((i >> 23) % 256) != 255)){
      printf("miss!!\n");
      printf("op:     ");
      print_bin(i);
      printf("output: ");
      print_bin(out);
      printf("ans:    ");
      print_bin(result);
      err++;
    }
    k++;
    if(fcmp(0,i) > 0 || fcmp(0x3fc90fda,i) < 0)
      k--;
  }
  printf("total: %d errors\n",err);
  
  
  return 0;
}
