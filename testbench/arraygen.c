#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OPEMAX 7

/*コマンドライン引数として
第一引数を演算、第二引数を生成するテストケースの数とする*/

/**/

extern unsigned int module_add(int num);
extern unsigned int module_mul(int num);
extern unsigned int module_div(int num);
extern unsigned int module_sqrt(int num);
extern unsigned int module_sin(int num);
extern unsigned int module_cos(int num);
extern unsigned int module_atan(int num);


int support(char *ope){
  char *oper[OPEMAX] = {"add","mul","div","sqrt","sin","cos","atan"};
  int i=0;
  while(i<OPEMAX){
    if(!strcmp(ope,oper[i]))
      return 1;
    i++;
  }
  return 0;
}

    
int main(int argc, char *argv[]){

  if(argc<3){
     printf("引数が足りない\n");
     return 1;
 }

  char *ope = argv[1];
  int num = atoi(argv[2]);

  if(!support(ope)){
    printf("sorry, %s is not supported\n", ope);
    return 0;
  }

    if(!(strcmp(ope,"add")))
      module_add(num);
    if(!(strcmp(ope,"mul")))
      module_mul(num);
    if(!(strcmp(ope,"div")))
      module_div(num);
    if(!(strcmp(ope,"sqrt")))
      module_sqrt(num);
    if(!(strcmp(ope,"sin")))
      module_sin(num);
    if(!(strcmp(ope,"cos")))
      module_cos(num);
    if(!(strcmp(ope,"atan")))
      module_atan(num);
    
  return 0;
}
