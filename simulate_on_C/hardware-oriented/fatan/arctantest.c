#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define N 1000
#define H 0.01

extern uint32_t fatan(uint32_t a);

int main(int argc,char* argv[]){
  
  printf("# name: atancurve\n");
  printf("# type: matrix\n");
  printf("# rows: %d\n",N);
  printf("# columns: 2\n");
  
  int i = 0;
  float f = -(H * N / 2);
  float out;
  uint32_t u,outu;

  while(i < N){
  
    memcpy(&u,&f,4);

    outu = fatan(u);

    memcpy(&out,&outu,4);
  
    printf("%f %f\n",f,out);
  
    i++;

    f += 0.01;
  }
  
  printf("\n\n");

  printf("# name: trueatan\n");
  printf("# type: matrix\n");
  printf("# rows: %d\n",N);
  printf("# columns: 2\n");

  f = -(H * N / 2);
  i=0;
  while(i < N){
  
    out = atanf(f);
  
    printf("%f %f\n",f,out);
  
    i++;

    f += 0.01;
  }

  return 0;
}
