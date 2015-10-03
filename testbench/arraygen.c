#include <stdio.h>
#include <string.h>

#define BINMAX 4;
#define OPEMAX 7;

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

int binary(char *ope){
  char *binope[BINMAX] = {"add","mul","div","exp"}
  int i=0;
  while(i<BINMAX){
    if(!strcmp(ope,binope[i]))
      return 1;
    i++;
  }
  return 0;
}

int support(char *ope){
  char *oper[OPEMAX] = {"add","mul","div","exp","root","square","inv"}
  int i=0;
  while(i<OPEMAX){
    if(!strcmp(ope,oper[i]))
      return 1;
    i++;
  }
  return 0;
}

    
int main(int argc, char *argv[]){

  char *ope = argv[1];
  char *opt = argv[2];

  unsigned int op1 = 0;
  unsigned int op2 = 0;
  unsigned int ans = 0;

  if(!support(ope)){
    printf("sorry, %s is not supported\n", ope);
    return 0;
  }

  while(1){
    if(opgen(&op1,&op2,ope,opt))//演算によって生成するオペランドを変える。
      break;
    printbin(op1);
    if(binary(ope))
      printbin(op2);
    ans = operate(op1,op2,ope);
    printbin(ans);
    printbin(gosa2(op1,op2,ans,ope));
  }
  return 0;
}
