CによるFPUのシュミレーション

floatのハードウェアによる型を入力uint32_tで表現。

software-orientedでは、ソフトウェア的な整数演算やループを用いて正確なものを実装

hardware-orientedでは、規格により許される誤差の範囲での出力をする(VHDLの模倣)。

テストは、hard版をsoft版と比較して計測

進捗...2015/10/20現在

--rev1

fadd...soft/hard実装、テスト済

fmul...soft/hard実装、テスト済

finv...soft/hard実装、テスト済

fsqrt...soft/hard実装、テスト済

fdiv...soft/hard実装、テスト済

fsin...soft/hard実装、テスト済

fcos...soft/hard実装、テスト中(0.5%誤差ケースあり)

fatan...under construction

--rev2

ftoi...under construction

itof...under construction

floor...under construction

Scraps...under construction

