#include <stdio.h>
#include <stdint.h>

extern void printbin(uint32_t a);

/*31petternを手書き*/
uint32_t itof(uint32_t i){
  
  
  uint32_t sgn = i >> 31;
  if(sgn)
    i = (1 << 31) - ((i << 1) >> 1);//絶対値に
  if(i >> 31)//-2^31
    return (1 << 31) + (157 << 23);
  
  if(i >> 30){
    if((i >> 6) % 2)//繰り上がりあり
      return (sgn << 31) + (157 << 23) + ((i << 2) >> 9) + 1;
    else
      return (sgn << 31) + (157 << 23) + ((i << 2) >> 9);
  }else if(i >> 29){
    if((i >> 5) % 2)
      return (sgn << 31) + (156 << 23) + ((i << 3) >> 9) + 1;
    else
      return (sgn << 31) + (156 << 23) + ((i << 3) >> 9);
  }else if(i >> 28){
    if((i >> 4) % 2)
      return (sgn << 31) + (155 << 23) + ((i << 4) >> 9) + 1;
    else
      return (sgn << 31) + (155 << 23) + ((i << 4) >> 9);
  }else if(i >> 27){
    if((i >> 3) % 2)
      return (sgn << 31) + (154 << 23) + ((i << 5) >> 9) + 1;
    else
      return (sgn << 31) + (154 << 23) + ((i << 5) >> 9);
  }else if(i >> 26){
    if((i >> 2) % 2)
      return (sgn << 31) + (153 << 23) + ((i << 6) >> 9) + 1;
    else
      return (sgn << 31) + (153 << 23) + ((i << 6) >> 9);
  }else if(i >> 25){
    if((i >> 1) % 2)
      return (sgn << 31) + (152 << 23) + ((i << 7) >> 9) + 1;
    else
      return (sgn << 31) + (152 << 23) + ((i << 7) >> 9);
  }else if(i >> 24)
    return (sgn << 31) + (151 << 23) + ((i << 8) >> 9);
  else if(i >> 23)
    return (sgn << 31) + (150 << 23) + ((i << 9) >> 9);
  else if(i >> 22)
    return (sgn << 31) + (149 << 23) + ((i << 10) >> 9);
  else if(i >> 21)
    return (sgn << 31) + (148 << 23) + ((i << 11) >> 9);
  else if(i >> 20)
    return (sgn << 31) + (147 << 23) + ((i << 12) >> 9);
  else if(i >> 19)
    return (sgn << 31) + (146 << 23) + ((i << 13) >> 9);
  else if(i >> 18)
    return (sgn << 31) + (145 << 23) + ((i << 14) >> 9);
  else if(i >> 17)
    return (sgn << 31) + (144 << 23) + ((i << 15) >> 9);
  else if(i >> 16)
    return (sgn << 31) + (143 << 23) + ((i << 16) >> 9);
  else if(i >> 15)
    return (sgn << 31) + (142 << 23) + ((i << 17) >> 9);
  else if(i >> 14)
    return (sgn << 31) + (141 << 23) + ((i << 18) >> 9);
  else if(i >> 13)
    return (sgn << 31) + (140 << 23) + ((i << 19) >> 9);
  else if(i >> 12)
    return (sgn << 31) + (139 << 23) + ((i << 20) >> 9);
  else if(i >> 11)
    return (sgn << 31) + (138 << 23) + ((i << 21) >> 9);
  else if(i >> 10)
    return (sgn << 31) + (137 << 23) + ((i << 22) >> 9);
  else if(i >> 9)
    return (sgn << 31) + (136 << 23) + ((i << 23) >> 9);
  else if(i >> 8)
    return (sgn << 31) + (135 << 23) + ((i << 24) >> 9);
  else if(i >> 7)
    return (sgn << 31) + (134 << 23) + ((i << 25) >> 9);
  else if(i >> 6)
    return (sgn << 31) + (133 << 23) + ((i << 26) >> 9);
  else if(i >> 5)
    return (sgn << 31) + (132 << 23) + ((i << 27) >> 9);
  else if(i >> 4)
    return (sgn << 31) + (131 << 23) + ((i << 28) >> 9);
  else if(i >> 3)
    return (sgn << 31) + (130 << 23) + ((i << 29) >> 9);
  else if(i >> 2)
    return (sgn << 31) + (129 << 23) + ((i << 30) >> 9);
  else if(i >> 1)
    return (sgn << 31) + (128 << 23) + ((i << 31) >> 9);
  else if(i)
    return (sgn << 31) + (127 << 23);

  return (sgn << 31) + 0;
}
  
