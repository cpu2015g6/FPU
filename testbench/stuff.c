#include <stdio.h>
#include <string.h>

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


float max2(float a, float b){
  if(a<b)
    return b;

  return a;
}


float max4(float a, float b, float c, float d){
  if(a<b)
    if(b<c)
      if(c<d)
	return d;
      else
	return c;
    else if(b<d)
      return d;
    else
      return b;
  else if(a<c)
    if(c<d)
      return d;
    else
      return c;
  else if(a<d)
    return d;

  return a;
}

int closed(float f1, float f2, float f3){
  unsigned int i1,i2,i3;
  memcpy(&i1,&f1,4);
  memcpy(&i2,&f2,4);
  memcpy(&i3,&f3,4);
 
  unsigned int i;
  
  i = (i1 >> 23) % 256;//f1の指数部,「f1が正規化数」は「i/=0,i/=255」と同じ
  if(!i || i == 255)
    return 0;
  
  i = (i2 >> 23) % 256;
  if(!i || i == 255)
    return 0;

  i = (i3 >> 23) % 256;
  if(!i || i == 255)
    return 0;
  return 1;
}

