  /*开辟7个存储空间*/
 1 int 0 7
  /*a=1,b=6,sum=0,i=1*/
 2 lit 0 1
 3 sto 0 3
 4 lit 0 6
 5 sto 0 4
 6 lit 0 0
 7 sto 0 6
 8 lit 0 1
 9 sto 0 5
  /*取i值与b值进行比较，若i<b为真则将0置栈顶，（此刻i是次栈顶，b为栈顶）*/
 10 lod 0 5
 11 lod 0 4
 12 opr 0 13
  /*条件跳转语句，若栈顶为0，则跳转26号指令，跳出循环*/
 13 jpc 0 26
  /*sum+=a*/
 14 lod 0 6
 15 lod 0 3
 16 opr 0 2
 17 sto 0 6
  /*输出i值到控制台*/
 18 lod 0 5
 19 opr 0 14
 20 opr 0 15
  /*i+=2，步长设定为2*/
 21 lod 0 5
 22 lit 0 2
 23 opr 0 2
 24 sto 0 5
  /*无条件跳转指令，回到10号指令，实现for循环*/
 25 jmp 0 10
  /*输出sum值至控制台*/
 26 lod 0 6
 27 opr 0 14
 28 opr 0 15
  /*i=b*/
 29 lod 0 4
 30 sto 0 5
  /*次栈顶i，栈顶为常数1，若i大小等于1，则将0存入栈顶*/
 31 lod 0 5
 32 lit 0 1
 33 opr 0 11
  /*条件跳转至43号指令*/
 34 jpc 0 43
  /*输出i值至控制台*/
 35 lod 0 5
 36 opr 0 14
 37 opr 0 15
  /*i-=2，步长为-2*/
 38 lod 0 5
 39 lit 0 2
 40 opr 0 3
 41 sto 0 5
  /*无条件跳转指令，实现for循环，回到31号指令*/
 42 jmp 0 31
  /*结束*/
 43 opr 0 0