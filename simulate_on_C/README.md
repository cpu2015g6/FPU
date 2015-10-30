CによるFPUのシュミレーション

floatのハードウェアによる型(32bit)を入力,出力uint32_tで表現。

software-orientedでは、ソフトウェア的な整数演算やループを用いて正確なものを実装

hardware-orientedでは、規格により許される誤差の範囲での出力をする(VHDLの模倣)。

テストは、hard版をsoft版と比較して計測

進捗...2015/10/29現在

--rev1

fadd...soft/hard実装、テスト済

fmul...soft/hard実装、テスト済

finv...soft/hard実装、テスト済

//finv

fsqrt...soft/hard実装、テスト済

//fsqrt
./test 0
-6    0
-5    0
-4    0
-3    29045
-2    1266070
-1    6527817 
+0    5708719 
+1    2281268 
+2    753347
+3    182669
+4    26527
+5    1753 
+6    1
