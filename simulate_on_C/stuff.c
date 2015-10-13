#include <stdio.h>
#include <stdint.h>
/*
  char to uint
  priorityencoder
  printbin
  encode
 */

uint32_t ctou(char *c){
  int i = 9;
  uint32_t u=0;
  while(i < 32){
    if(c[i] == '1')
      u += 1 << (31 - i);
    i++;
  }
  return u;
}

int pencoder(uint32_t x){//software instruction
  int i = 31;
  while(i >= 0){
    if(x >> i)
      break;
    i--;
  }
  return 31 - i;
}

void printbinn(unsigned int a){
  printf("\"");
  int i=31;
  while(i+1){
    if((a >> i) % 2)
      printf("1");
    else
      printf("0");
    i--;
  }
  printf("\",\n");
}
 
void printbin(unsigned int a){
  printf("\"");
  int i=31;
  while(i+1){
    if((a >> i) % 2)
      printf("1");
    else
      printf("0");
    i--;
  }
  printf("\",\n");
}

int encode(uint32_t a){
  int sgn = a >> 31;
  int exp = (a >> 23) % 256;

  if(!(a << 1))
    return 6;//0

  if(exp == 255){
    if(a << 9)
      return 7;//NaN
    else if(sgn)
      return 5;//-Inf
    else
      return 4;//+Inf
 }
  
  if(exp){//Normalized
    if(sgn)
      return 1;//-N
    else
      return 0;//+N
 }
  
  if(sgn)
    return 3;//-DN

  return 2;//+DN
}
