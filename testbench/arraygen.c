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
    
int main(int argc, char *argv[]){

  if(!strcmp(argv[1],"-h")){
    printf("  arraygenの使い方\n");
    printf("FPUのテストベンチのために、オペランドと答え,許される誤差を羅列したします。\n");
    printf("テストベンチにコピペして下さい(textio使いたかったらこのcファイルをちょっといじって下さい)");
    printf("このcファイルをコンパイルして、実行時に引数としてadd,mul,div,root,squareのいずれかを与えれば動きます");
    printf("生成ファイルはあくまでコンパイラとマシンのFPUに依存してるのでご了承下さい");
    printf("それではCPU実験頑張りましょう~\n");
    return 0;
  }

  unsigned int op1 = 0;
  unsigned int op2 = 0;
  unsigned int ans = 0;

  
  while(){
    printbin(op1);
    printbin(op2);
    printbin(ans);
    printbin((unsigned int)gosa(op1,op2,ans));
  }
  return 0;
}
