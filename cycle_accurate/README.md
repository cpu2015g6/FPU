# tomasulo模倣simulator

## 原理

- 以下のコンポーネントからなる
- program_loaderやtransmitterなどはない。
- 第一引数に実行バイナリを与える。
- 命令メモリの0番地から実行
- dataセグメントはなし
- 
実行例
./sim a.bin(バイナリ)

main

|- recvif

|- sramif

|- cpu_top

   |- alu

   |- fpu

   |- branch

   |- mem

