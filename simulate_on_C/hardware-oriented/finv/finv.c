#include <stdio.h>


extern int fcmp(uint32_t a, uint32_t b);
extern void printbin(uint32_t x);
extern int encode(uint32_t a);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern uint32_t fmul(uint32_t a, uint32_t b);
extern int pencoder(uint32_t a);

uint32_t invsub(uint32_t a){

  uint32_t sgn = a >> 31;//sgn<=a(31)
  uint32_t expa = (a >> 23) % 256;//expa<=a(30 downto 23)
  uint32_t mana = ((a << 23) >> 23);//mana<=a(22 downto 0)
  //ゲタ表現のまま
  uint32_t exp = 512 - expa - 3;//exp<=not(expa) - 2
  
  
  if(!(mana << 9))//if mana = "00..0"
    return (sgn << 31) + ((exp + 1) << 23);//ans<=sgn & (exp + 1) & "00..0"
  
  if(!(mana >> 12))//if mana(22 downto 12) = "00..0"
    return (sgn << 31) + (exp << 23) + (2*(4194304 - mana));//ans<=sgn & (exp + 1) & (not(mana(21 downto 0)) + 2) & "0"
  /*
    1まわりのTaylor展開により
    1 / man = 1 - 1! / (man-1) + 2! / (man-1)^2 - 3! / (man-1)^3...
    から、manの上位11桁が0のときは、下位3桁の誤差の範囲で1 - 1! / (man-1)で解が与えられる
   */
  

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
  if(typa == 4)// 1 / +Inf
    return 0;
  if(typa == 5)// 1 / -Inf
    return 0x80000000;
  if(typa > 1)
    return dninv(a);
  
  return invsub(a);
}
  
