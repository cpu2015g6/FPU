#include <stdio.h>
#include <stdint.h>

uint32_t vector(uint32_t a, int i, int j);

uint32_t revfadd(uint32_t a, uint32_t b){
  
  uint32_t sgn;
  uint32_t exp;
  uint32_t man1,man2,man3;
  uint32_t del;
  uint32_t prior,infer;

  if(vector(a,30,0) > vector(b,30,0)){
    prior=a;
    infer=b;
  }else{
    prior=b;
    infer=a;
  }
  sgn=vector(prior,31,31);
  exp=vector(prior,30,23);
  del=exp - vector(infer,30,23);
  man1=(vector(prior,22,0) << 1) + (1 << 24);
  
  if(!del)
    man2=(vector(infer,22,0) << 1) + (1 << 24);
  else if(del < 24)
    man2=vector(infer,22,del-1) + (1 << (24-del));
  else if(del == 24)
    man2=1;
  else
    man2=0;
 
  man3=man1-man2;
 
  int i = 24;

  while(i >= 0){
    if(vector(man3,i,i))
      break;
    i--;
  }
  if(i == 24)
    return (sgn << 31) + (exp << 23) + vector(man3,23,1);
  if(!i)
    return (sgn << 31) + ((exp-24) << 23);
  if(i == -1)
    return 0;

  return (sgn << 31) + ((exp-(24-i)) << 23) + (vector(man3,i-1,0) << (23-i));
}


uint32_t absfadd(uint32_t a, uint32_t b){
  
  uint32_t sgn = vector(a,31,31);
  uint32_t exp;
  uint32_t man1,man2,man3;
  uint32_t del;
  uint32_t prior,infer;
  
  if(vector(a,30,0) > vector(b,30,0)){
    prior=a;
    infer=b;
  }else{
    prior=b;
    infer=a;
  }
  exp=vector(prior,30,23);
  del=exp - vector(infer,30,23);
  man1=(vector(prior,22,0) << 1) + (1 << 24);
  if(!del)
    man2=(vector(infer,22,0) << 1) + (1 << 24);
  else if(del < 24)
    man2=(vector(infer,22,del-1)) + (1 << (24-del));
  else if(del == 24)
    man2=1;
  else
    man2=0;
  
  man3=man1+man2;
  
  if(vector(man3,25,25))
    return (sgn << 31) + ((exp+1) << 23) + vector(man3,24,2);
  
  return (sgn << 31) + (exp << 23) + vector(man3,23,1);
}

uint32_t fadd(uint32_t a, uint32_t b){

  int flag = (vector(a,31,31) + vector(b,31,31)) % 2; // xor
  
  if(flag)
    return revfadd(a,b);
 
  return absfadd(a,b);
}
