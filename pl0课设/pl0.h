/*PL/0编译系统C版本头文件pl0.h*/

#include <stdio.h>

typedef enum
{
	false,
	true
} bool;

#define norw 20	  /* 关键字个数 */	//change 13+5+2
#define txmax 100 /* 名字表容量 */
#define nmax 14	  /* number 的最大位数 */
#define al 10	  /* 符号的最大长度 */
#define amax 2047 /* 地址上界 */
#define levmax 3  /* 最大允许过程嵌套声明层数 [0, levmax] */
#define cxmax 200 /* 最多的虚拟机代码数 */

/* 符号 */
enum symbol
{
	nul, ident, number, plus, minus,
	times, slash, oddsym, eql, neq,
	lss, leq, gtr, geq, lparen, rparen,
	comma, semicolon, period, becomes,
	beginsym, endsym, ifsym, thensym, whilesym,
	writesym, readsym, dosym, callsym, constsym,
	varsym, procsym,
	/*添加新的保留字*/
	forsym, tosym, downtosym, returnsym, elsesym,
	peql/*+=*/, meql/*-=*/, dplus/*++*/, dminus/*--*/,
	/*-----add_up---------*/
	timeseql, slasheql, charsym, realsym,colon/*:*/,
	/*------add_bottom----*/
};

#define symnum 46	//change  32+9+5

/* 名字表中的类型 */
enum object
{
	constant,
	variable,
	procedur,
	/*------add_up----------*/
	charcon,//字符型
	realcon,//实型
	array,	//数组类型
	/*-------add_bottom--------*/
};

/* 虚拟机代码 */
enum fct
{
	lit, opr, lod, sto,
	cal, inte, jmp, jpc,
	/*------add_up0=-------*/
	//数组加两个存取方式
	sta,lda,
	/*-----add_bottom------*/
};

#define fctnum 10		//修改 8+2

/* 虚拟机代码结构 */
struct instruction
{
	enum fct f; /* 虚拟机代码指令 */
	int l;		/* 引用层与声明层的层次差 */
	int a;		/* 根据 f 的不同而不同 */
};

FILE* fas; /* 输出名字表 */
FILE* fa;  /* 输出虚拟机代码 */
FILE* fa1; /* 输出源文件及其各行对应的首地址 */
FILE* fa2; /* 输出结果 */

bool listswitch;  /* 显示虚拟机代码与否 */
bool tableswitch; /* 显示名字表与否 */

char ch;		 /* 读取字符的缓冲区，getch 使用 */
enum symbol sym; /* 当前符号 */
char id[al + 1]; /* 当前 ident ，多出的一个字节用于存放 0 */
int num;		 /* 当前 number */

int cc, ll; /* getch 使用的计数器，cc 表示当前字符 ch 的位置 */
int cx;		/* 虚拟机代码指针，取值范围为 [0, cxmax - 1] */

char line[81];	/* 读取行缓冲区 */
char a[al + 1]; /* 临时符号，多出的一个字节用于存放 0 */

struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */
char word[norw][al];			/* 保留字 */
enum symbol wsym[norw];			/* 保留字对应的符号值 */
enum symbol ssym[256];			/* 单字符的符号值 */
char mnemonic[fctnum][5];		/* 虚拟机代码指令名称 */

bool declbegsys[symnum]; /* 表明声明开始的符号集合 */
bool statbegsys[symnum]; /* 表明语句开始的符号集合 */
bool facbegsys[symnum];	 /* 表明因子开始的符号集合*/

/* 名字表结构 */
struct tablestruct
{
	char name[al];	  /* 名字 */
	enum object kind; /* 类型： const/var/array/procedure/char/real */
	int val;		  /* 数值，仅 const 使用 */
	int level;		  /* 所处层，仅 const 不使用 */
	int adr;		  /* 地址，仅 const 不使用 */
	int size;		  /* 需要分配的数据区空间，仅 procedure 使用 */
	/*--------add_up-----------*/
	/*下界：仅数组需要使用，相对起始位置*/
	int low;		
	/*-------add_bottom--------*/
};

struct tablestruct table[txmax]; /* 名字表 */

FILE* fin;
FILE* fout;
char fname[al];
int err; /* 错误计数器 */

/* 当函数中会发生 fatal error 时，返回 -1 告知调用它的函数，最终退出程序 */
#define getsymdo      if (-1 == getsym()) return -1
#define getchdo       if (-1 == getch()) return -1
#define testdo(a, b, c)   if (-1 ==  test(a, b, c)) return -1
#define gendo(a, b, c)    if (-1 == gen(a, b, c)) return -1
/*-----------add_up------------*/
#define expressionArraydo(a,b,c,d)	if(-1==expressionArray(a,b,c,d)) return -1;
/*----------add_bottom---------*/
#define expressiondo(a, b, c)   if (-1 == expression(a, b, c)) return -1
#define factordo(a, b, c)       if (-1 == factor(a, b, c)) return -1
#define termdo(a, b, c)        if (-1 == term(a, b, c)) return -1
#define conditiondo(a, b, c)        if (-1 == condition(a, b, c)) return -1
#define statementdo(a, b, c)        if (-1 ==  statement(a, b, c)) return -1
#define constdeclarationdo(a, b, c)        if (-1 == constdeclaration(a, b, c)) return -1
#define vardeclarationdo(a, b, c)        if (-1 ==  vardeclaration(a, b, c)) return -1
/*-----------add_up------------*/
#define chardeclarationdo(a,b,c)              if(-1==chardeclaration(a,b,c))return -1
#define realdeclarationdo(a,b,c)			  if(-1==realdeclaration(a,b,c))return -1	
/*-----------add_bottom--------*/

void error(int n);
int getsym();
int getch();
void init();
int gen(enum fct x, int y, int z);
int test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
int block(int lev, int tx, bool* fsys);
void interpret();
int factor(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
/*------add_up---------*/
/* 处理数组数据 */
int expressionArray(bool* fsys, int* ptx, int lev,int index);
/*------add_botton-----*/
int statement(bool* fsys, int* ptx, int lev);
void listcode(int cx0);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);
/*------add_up---------*/
/* 处理数组数据 */
void enterArray(int* ptx, int lev, int* pdx, int start, int end, char* id);
/*------add_botton-----*/
int base(int l, int* s, int b);
/*------add_up---------*/
/* 判断是否是常量 */
int isIndex(int index);
/*------add_botton-----*/
