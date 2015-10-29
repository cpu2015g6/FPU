#include <stdio.h>
#include <stdint.h>
/*
  char to uint
  priorityencoder
  printbin
  encode
 */

void printtable(int *table){
  int i=0;
  int j,k,num,a;
  char sgn;
  while(i < 64){
   if(table[i])
     break;
   i++;
   }
   j=i;
   i=127;
  while(i > 64){
   if(table[i])
     break;
   i--;
   }
   k=i; 
   if((64 - j) < (k - 64))
     num = k-64;
   else
     num = 64-j;

  i=64-num;
  while(i < 65+num){
   if(i < 64){
     sgn='-';
     a=64-i;
   }else{
     sgn='+';
     a=i-64;
   }
   printf("%c%-3d  %-10d\n",sgn,a,table[i]);
   i++;
 }
}

void printbin(uint32_t a){
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

uint32_t vector(uint32_t a, int i, int j){
  if(i < j)
    return 0;

  uint32_t ans=0;
  int k = 0;

  while(j < i+1){
   if((a >> j) % 2)
    ans += 1 << k;
   j++;
   k++;
  }

  return ans;
}

uint32_t ctou(char *c){
  int i = 0;
  uint32_t u=0;
  while(i < 32){
    if(c[i] == '1')
      u += 1 << (31 - i);
    i++;
  }
  return u;
}

uint32_t ctou23(char *c){
  int i = 0;
  uint32_t u=0;
  while(i < 23){
    if(c[i] == '1')
      u += 1 << (22-i);
    i++;
    //printbin(u);
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

void printbinn(uint32_t a){
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
 


void print23bin(uint32_t a){
  printf("\"");
  int i=22;
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
