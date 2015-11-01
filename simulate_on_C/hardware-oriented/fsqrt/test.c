#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "sqrtromdummy.h"

extern void printtable(int *table);
extern void printbin(uint32_t a);
extern uint32_t fsqrt(uint32_t a, int offset);
extern uint32_t fsqrt_s(uint32_t a);
extern int distri(int *table);
extern uint32_t ctou13(char* c);
extern uint32_t ctou23(char* c);

extern uint32_t fmul_s(uint32_t a, uint32_t b);
extern int fcmp(uint32_t a, uint32_t b);
extern void printbinn(unsigned int a);
extern void print23bin(unsigned int a);
extern void print13bin(unsigned int a);
extern void print_13bin(uint32_t a);

void clear(int *table){
  int i=0;
  while(i<128){
    table[i]=0;
    i++;
  } 
}

void incdecprint(char mem[1024][23],int a,int offset){
  print23bin(ctou23(mem[offset]) + a);
}

void incdecprint2(char mem[1024][13],int best,int offset){
  print_13bin(ctou13(mem[offset]) + best);
}
  

int lub(int *table){
  int i=127;
  while(i){
    if(table[i])
      break;
    i--;
  }
  return i - 64;
}

int glb(int *table){
  int i=0;
  while(i < 128){
    if(table[i])
      break;
    i++;
  }
  return i - 64;
}

uint32_t incdec(uint32_t a, int b){
  if(b > 0){
    while(b--)
      a += 1 << 10;
    return a;
  }else{
    while(b++)
      a -= 1 << 10;
    return a;
  }
  return a;
}
    

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
  int aaa[1024];
  int k=0;
  int m=0;
  int del = 0;
  int table[128];

  if(argc < 2){
   printf("few args!!\n");
   return 1;
  }

  clear(table);

  int offset = atoi(argv[1]);
  int offset_term = atoi(argv[2]);
  
  int j=0 - (atoi(argv[3]));
  int dist=1000;
  int best=0;

  while(offset < offset_term){

    dist=1000;
    j=0 - (atoi(argv[3]));

    while(j < atoi(argv[3]) + 1){

      k=0;
      i=(127<<23) + (offset << 14);

      while(k < 16384){
	result=fsqrt_s(i);
	out = fsqrt(i,j);
	del = out - result;
	table[del+64] = table[del+64]+1;
	k++;
	i++;
      }
      
      printtable(table);
      return 1;
      

      if(distri(table) < dist){
	best = j;
	dist = distri(table);
      }

      clear(table);
      
      j++;
    }
    
    k=0;
    i=(127<<23) + (offset << 14);
     while(k < 16384){
	result=fsqrt_s(i);
	out = fsqrt(i,best);
	del = out - result;
	table[del+64] = table[del+64]+1;
	k++;
	i++;
      }
     
     //print23bin(ctou(dummy[offset]));
     //printtable(table);
     //printf("%d\n",lub(table));
     aaa[offset] = 0 - ((lub(table)+glb(table)) /2);
    
     //clear(table);
    
     if(dist < 6){
      fprintf(stderr,"clear: offset=%d,dist=%d,best=%d,glb=%d,lub=%d\n",offset,dist,best,glb(table),lub(table));
    }else{
      fprintf(stderr,"failed......: offset=%d,dist=%d,best=%d,glb=%d,lub=%d\n",offset,dist,best,glb(table),lub(table));
      }
    
     /*if(offset < 512)
       incdecprint2(dummy2,best,512+offset);
     else
       incdecprint2(dummy2,best,offset-512);*/
     
     clear(table);
      
     fprintf(stderr,"sinchoku...%d/%d\n",offset,atoi(argv[2]));

     offset++;
  }
  offset=atoi(argv[1]);
  while(offset < atoi(argv[2])){
    if(offset < 512)
     incdecprint(dummy,aaa[512 + offset],offset);
    else
     incdecprint(dummy,aaa[offset - 512],offset);
    offset++;
  }

	//printf("best case: %d dist: %d\n",best,dist);
  
  
  return 0;
}
