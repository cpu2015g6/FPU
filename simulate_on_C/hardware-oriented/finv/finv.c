#include <stdio.h>
#include <stdint.h>
#include "invrom.h"

extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(uint32_t x);
extern int encode(uint32_t a);
extern int pencoder(uint32_t a);
extern uint32_t ctou(char *c);

uint32_t fmul_inv(uint32_t data2, uint32_t del, int flag){//data2とdelを掛け算し、13桁で返す
  uint32_t d1,d2,d3,de1,de2,de3;
  uint32_t r11,r12,r21,r22,r13,r31;
  uint32_t ans;
  d1=data2>>19;
  d2=(data2>>15)%16;
  d3=(data2>>11)%16;
  de1=del>>8;
  de2=(del>>4)%16;
  de3=del%16;
  
  r11=d1*de1;
  r12=d1*de2;
  r21=d2*de1;
  r22=d2*de2;
  r13=d1*de3;
  r31=d3*de1;
  
  ans=del + (r11 << 4) + (r12 + r21) + ((r22 + r13 + r31) >> 4);
  
  if(flag)
    return ans >> 1;
  return ans;
}


uint32_t invsub(uint32_t a){

  uint32_t sgn = a >> 31;//sgn<=a(31)
  uint32_t expa = (a >> 23) % 256;//expa<=a(30 downto 23)
  uint32_t exp = 256 - expa - 3;//exp<=253-expa
  int addr = (a << 9) >> 21;//addr<=a(22 downto 12)
  uint32_t data = ctou(rom[addr]);//data<=rom(conv_integer(addr))
  uint32_t data2 = ctou(rom2[addr]);//data2<=rom(conv_integer(addr))
  uint32_t del = (a % 4096);//del<= a(11 downto 0)
  //printbin(a);
  //printbin(data);
  //printbin(del);
  
  
  if(!(a << 9))//if a(22 downto 0) = "00..0" then
    return (sgn << 31) + ((exp + 1) << 23);//ans<=sgn & (exp+1) & "00..0"

  if(addr < 849)
    return (sgn << 31) + (exp << 23) + data - fmul_inv(data2,del,0);/*
								       ans<=sgn & exp & (data - fmul_invans)
								       fmul_invop1<=data2
								       fmul_invop2<=del //delは12桁
								       実装上del,fmul_invop2は12bit.
								       それ以外は23bitで問題ない
								     */
  return (sgn << 31) + (exp << 23) + data - fmul_inv(data2,del,1);//data2の指数部が1つ下がる

  /*
    cまわりのTaylor展開により
    1 / a = 1/c - (1! / c^2) (a-c) + (2! / c^3) (a-c)^2 - (3! / c^4) (a-c)^3...
    から、(a-c)のman上位11桁が0のときは、下位3桁の誤差の範囲で1/c - (1 / c^2) (a-c)で解が与えられる
    rom,rom2には、2^11パターンの1/|c|,1/|c|^2のデータが入っている。
   */
}
  

uint32_t finv(uint32_t a){
  
  int typa = encode(a);
  int nega=0;

  if(typa == 6 && (a >> 31))//-0の拡張
    nega=1;

  if(typa == 7)
    return 0xffc00000;
  if(nega)// 1 / -0
    return 0xff800000;
  if(typa == 6)
    return 0x7f800000;
  if(typa > 1)
    return ((a >> 31) << 31) + 0x7f800000;
  
  return invsub(a);
}
  
