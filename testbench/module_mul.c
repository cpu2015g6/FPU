#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define abs(x) ((x) < 0 ? - (x) : (x))

extern void printbin(unsigned int a);
extern float max2(float a, float b);
extern int closed(float f1, float f2, float f3);

void module_mul(int num){
  unsigned int op1,op2,inf,sup;
  int i;
  float f1,f2,ans,gosa;

  unsigned int idel = 0x00800000; /*floatで2^-126*/
  float del;
  memcpy(&del,&idel,4);

  unsigned int idel2 = 0x35000000; /*floatで2^-22*/
  float del2;
  memcpy(&del2,&idel2,4);

  printf("mul operate\ngenerate %d cases\n",num);
  
  srand((unsigned) time(NULL));

  do{
    i = rand();
    memcpy(&op1,&i,4);
    memcpy(&f1,&i,4);
    i = rand();
    memcpy(&op2,&i,4);
    memcpy(&f2,&i,4);

    ans = f1 * f2; /*simple mul*/
    if(closed(f1,f2,ans)){/*オペランドと答えが正規化数か確認、だめならやり直し*/
      gosa = max2(ans*del2,del);
  
      f1 = ans - gosa;
      f2 = ans + gosa;
      memcpy(&inf,&f1,4);
      memcpy(&sup,&f2,4);

      printbin(op1);
      printbin(op2);
      printbin(inf);
      printbin(sup);
      num--;
    }
  }while(num > 0);
  
}