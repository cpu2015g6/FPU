#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "invromdummy.h"

extern void printtable(int *table);
extern void printbin(uint32_t a);
extern uint32_t finv(uint32_t a, int offset);
extern uint32_t finv_s(uint32_t a);
extern int distri(int *table);
extern uint32_t ctou13(char* c);
extern uint32_t ctou23(char* c);

extern void printbinn(unsigned int a);
extern void print23bin(unsigned int a);
extern void print13bin(unsigned int a);
extern void print_13bin(uint32_t a);

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
  int failed=0;
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

  i=0;
  while(i<128){
    table[i]=0;
    i++;
  }
  int offset = atoi(argv[1]);
  int offset_term = atoi(argv[2]);
  i=(127<<23) + (offset << 13);

  int j=0 - (atoi(argv[3]));
  int dist=1000;
  int best=0;

  while(offset < offset_term){

    dist=1000;
    j=0 - (atoi(argv[3]));

    while(j < atoi(argv[3]) + 1){

      k=0;
      i=(127<<23) + (offset << 13);

      while(k < 8192*1024){
	result=finv_s(i);
	out = finv(i,j);
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

      m=0;
      while(m<128){
	table[m]=0;
	m++;
      }
      
      j++;
    }
    
    k=0;
    i=(127<<23) + (offset << 13);
     while(k < 8192){
	result=finv_s(i);
	out = finv(i,best);
	del = out - result;
	table[del+64] = table[del+64]+1;
	k++;
	i++;
      }

     //print23bin(ctou(dummy[offset]));
     //printtable(table);
     //printf("%d\n",lub(table));
     aaa[offset] = 0 - ((lub(table)+glb(table)) /2);
     fprintf(stderr,"aaa: %d\n",aaa[offset]);
     /*m=0;
     while(m<128){
       table[m]=0;
       m++;
     }*/
    
     if(dist < 10){
      fprintf(stderr,"clear: offset=%d,dist=%d,best=%d,glb=%d,lub=%d\n",offset,dist,best,glb(table),lub(table));
    }else{
      fprintf(stderr,"FAILED...: offset=%d,dist=%d,best=%d,glb=%d,lub=%d\n",offset,dist,best,glb(table),lub(table));
      failed++;
      }
    fprintf(stderr,"sinchoku...%d/%d\n",offset-atoi(argv[1]),atoi(argv[2])-atoi(argv[1]));
    //incdecprint2(dummy2,best,offset);

      m=0;
      while(m<128){
	table[m]=0;
	m++;
      }

      offset++;
  }
  offset=atoi(argv[1]);
  while(offset < atoi(argv[2])){
    incdecprint(dummy,aaa[offset],offset);
    offset++;
  }

	//printf("best case: %d dist: %d\n",best,dist);
  fprintf(stderr,"failed cases: %d/%d\n",failed,atoi(argv[2])-atoi(argv[1]));
  
  return 0;
}
