# -PL0
## 实验要求
对PL/0作以下修改扩充：  
（1）增加单词：保留字：FOR，TO，DOWNTO，+=，-=，++，--；RETURN  
        要求：词法识别即可  
（2）替换单词：不等号# 改为 < >  
（3）增加条件语句的ELSE子句  

## 课设要求

（1）扩充赋值运算：+= 和 -=  
（2）扩充语句（Pascal的FOR语句）:  
①FOR <变量>:=<表达式> TO <表达式> DO <语句>  
②FOR <变量>:=<表达式> DOWNTO <表达式> DO <语句>  
     其中，语句①的循环变量的步长为1，语句②的循环变量的步长为-1。  
选做内容（成绩评定范围扩大到：“优”和“良”）  
（1）增加运算：++ 和 --。  
（2）增加类型：① 字符类型;   ② 实数类型。  
（3）扩充函数：① 有返回值和返回语句；② 有参数函数。  
（4）增加一维数组类型（可增加指令）。  

##  设计过程

1、新添加的保留字符号（9个）：

forsym，tosym，downtosym，returnsym，elsesym，（5个关键字）

peql（+=）,meql（-=）, dplus（++）, dminus（--）, 

<img src="./pictures/pl0函数定义层次结构.png" style="zoom: 80%;" />

（来源：[北航 编译实践 PL/0文法 - 姜楠 - 博客园 (cnblogs.com)](https://www.cnblogs.com/ZJUT-jiangnan/p/3494501.html)）

| 单词符号  | 类型                                  |
| --------- | ------------------------------------- |
| +         | plus                                  |
| -         | minus                                 |
| *         | times                                 |
| /         | slash                                 |
| (         | lparen                                |
| )         | rparen                                |
| =         | eql                                   |
| ,         | comma                                 |
| .         | period                                |
| **#**     | **neq** **（在本次实验中修改成 <>）** |
| ;         | semicolon                             |
| begin     | beginsym                              |
| call      | callsym                               |
| const     | constsym                              |
| do        | dosym                                 |
| end       | endsym                                |
| if        | ifsym                                 |
| odd       | oddsym                                |
| procedure | proceduresym                          |
| read      | readsym                               |
| then      | thensym                               |
| var       | varsym                                |
| while     | whilesym                              |
| ...       | ...                                   |

| 新添加单词符号 | 类型          |
| -------------- | ------------- |
| **for**        | **forsym**    |
| **to**         | **tosym**     |
| **downto**     | **downtosym** |
| **return**     | **returnsym** |
| **else**       | **elsesym**   |
| **+=**         | **peql**      |
| **-=**         | **meql**      |
| **++**         | **dplus**     |
| **--**         | **dminus**    |

（黑体单词符号是本次实验中添加内容）

语法分析：

![](./pictures/语法分析.png)

