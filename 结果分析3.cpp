 /*开辟6个存储单元*/
 1 int 0 6
 /*a=10*/
 2 lit 0 10
 3 sto 0 3
 /*读入控制台值，输入b，测试输入为2*/
 4 opr 0 16
 5 sto 0 4
 /*读入控制台值，输入c，测试输入为6*/
 6 opr 0 16
 7 sto 0 5
 /*取a、b值入栈*/
 8 lod 0 3
 9 lod 0 4
 /*a、b相加，值存入a地址，此时a=12*/
 10 opr 0 2
 11 sto 0 3
 /*a值输出控制台，应为12*/
 12 lod 0 3
 13 opr 0 14
 14 opr 0 15
 /*取a、c值入栈*/
 15 lod 0 3
 16 lod 0 5
  /*a、b相减，值存入a地址，此时a=6*/
 17 opr 0 3
 18 sto 0 3
 /*a值输出控制台，应为6*/
 19 lod 0 3
 20 opr 0 14
 21 opr 0 15
 22 opr 0 0
