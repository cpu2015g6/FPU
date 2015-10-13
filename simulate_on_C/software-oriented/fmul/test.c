#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern int fcmp(uint32_t a, uint32_t b);
extern uint32_t fmul_s(uint32_t a, uint32_t b);
extern void printbinn(uint32_t a);
extern int encode(uint32_t a);

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
  uint32_t j;
  uint32_t result;
  uint32_t fm;
  int num = atoi(argv[1]);
  int k=0;
  double d1,d2,dr;
  float f1,f2,fr;
  int err = 0;
  
  srand((unsigned) time(NULL));

  while(k < num){
    i = rand();
    j = rand();
    i += (rand() % 2) << 31;
    j += (rand() % 2) << 31;
    memcpy(&f1,&i,4);
    d1 = (double)f1;
    memcpy(&f2,&j,4);
    d2 = (double)f2;
    dr = d1 * d2;
    fr = (float)dr;
    memcpy(&result,&fr,4);
    fm = fmul_s(i,j);
    if(encode(fm) < 2){
    printbinn(i);
    printbinn(j);
    printbinn(fm-2);
    printbinn(fm+2);
    }
    if( ((i >> 23) % 256) && ((j >> 23) % 256) //オペランドは正規化数に限る
        && (fcmp(fm,(result + 3)) == (fcmp(fm,(result - 3))))
	&& (((fm >> 23) % 256) + ((result >> 23) % 256))//非正規化数の丸め
	&& (((i >> 23) % 256) != 255) && (((j >> 23) % 256) != 255)){
      printf("miss!!\n");
      printf("op1:    ");
      print_bin(i);
      printf("op2:    ");
      print_bin(j);
      printf("output: ");
      print_bin(fm);
      printf("ans:    ");
      print_bin(result);
      err++;
    }
    k++;
  }
  printf("total: %d errors\n",err);
  
  return 0;
}
