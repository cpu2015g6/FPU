# tomasulo模倣simulator

## 原理

- 以下のコンポーネントからなる
- program_loaderやtransmitterなどはない。
- 第一引数に実行バイナリ,第二引数にファイルを与える。

main
|- recvif
|- sramif
|- cpu_top
   |- alu
   |- fpu
   |- branch
   |- mem

