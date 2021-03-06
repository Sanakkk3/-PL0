//18级信息安全2班

/*编译和运行环境：
*1 Visual C++6.0,Visual C++.NET and Visual C++.NET 2003
*  WinNT,Win2000,WinXP and Win2003
*2 gcc version 3.3.2 20031022(Red Hat Linux 3.3.2-1)
*  Redhat Fedora core 1
*  Intel 32 platform
*使用方法:
*运行后输入PL/0 源程序文件名
*回答是否输出虚拟机代码
*回答是否输出名字表
*fa.tmp 输出虚拟机代码
*fa1.tmp  输出源文件及其各行对应的首地址
*fa2.tmp  输出结果
*fas.tmp  输出名字表
*/

#pragma warning(disable : 4996)//这个是额外添加的，关闭VS2019中的安全警告，不用VS2019可以删掉。

#include<stdio.h>
#include"pl0.h"
#include"string.h"
/*解释执行时使用的栈*/
#define stacksize 500
int main()
{
    bool nxtlev[symnum];
    printf("Input pl/0 file ?");
    scanf("%s", fname);                                     /*输入文件名*/
    fin = fopen(fname, "r");
    if (fin)
    {
        printf("List object code ?(Y/N)");                /*是否输出虚拟机代码*/
        scanf("%s", fname);
        listswitch = (fname[0] == 'y' || fname[0] == 'Y');
        printf("List symbol table ? (Y/N)");             /*是否输出名字表*/
        scanf("%s", fname);
        tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
        fa1 = fopen("fa1.tmp", "w");
        fprintf(fa1, "Iput pl/0 file ?");
        fprintf(fa1, "%s\n", fname);
        init();                                          /*初始化*/
        err = 0;
        cc = cx = ll = 0;
        ch = ' ';
        if (-1 != getsym())
        {
            fa = fopen("fa.tmp", "w");
            fas = fopen("fas.tmp", "w");
            addset(nxtlev, declbegsys, statbegsys, symnum);
            nxtlev[period] = true;
            if (-1 == block(0, 0, nxtlev))           /*调用编译程序*/
            {
                fclose(fa);
                fclose(fa1);
                fclose(fas);
                fclose(fin);
                printf("\n");
                return 0;
            }
            fclose(fa);
            fclose(fa1);
            fclose(fas);
            if (sym != period)
            {
                error(9);
            }
            if (err == 0)
            {
                fa2 = fopen("fa2.tmp", "w");
                interpret();
                fclose(fa2);
            }
            else
            {
                printf("Errors in pl/0 program");
            }
        }
        fclose(fin);
    }
    else
    {
        printf("Can't open file! \n");
    }
    printf("\n");
    system("pause");
    return 0;
}

/*
*初始化
*/
void init()
{
    int i;
    /*设置单字符符号*/
    for (i = 0; i <= 255; i++)
    {
        ssym[i] = nul;
    }
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['='] = eql;
    ssym[','] = comma;
    ssym['.'] = period;
    //ssym['#']=neq;  //去掉原先单字符‘#’的设置部分
    ssym[';'] = semicolon;
    /*设置保留字名字,按照字母顺序,便于折半查找*/
    strcpy(&(word[0][0]), "begin");
    strcpy(&(word[1][0]), "call");
    strcpy(&(word[2][0]), "char");      //add
    strcpy(&(word[3][0]), "const");
    strcpy(&(word[4][0]), "do");
    strcpy(&(word[5][0]), "downto");        //add
    strcpy(&(word[6][0]), "else");          //add
    strcpy(&(word[7][0]), "end");
    strcpy(&(word[8][0]), "for");               //add
    strcpy(&(word[9][0]), "if");
    strcpy(&(word[10][0]), "odd");
    strcpy(&(word[11][0]), "procedure");
    strcpy(&(word[12][0]), "read");
    strcpy(&(word[13][0]), "return");           //add
    strcpy(&(word[14][0]), "real");             //add
    strcpy(&(word[15][0]), "then");
    strcpy(&(word[16][0]), "to");               //add
    strcpy(&(word[17][0]), "var");
    strcpy(&(word[18][0]), "while");
    strcpy(&(word[19][0]), "write");

    /*设置保留字符号*/
    wsym[0] = beginsym;
    wsym[1] = callsym;
    wsym[2] = charsym;      //add
    wsym[3] = constsym;
    wsym[4] = dosym;
    wsym[5] = downtosym;            //add
    wsym[6] = elsesym;              //add
    wsym[7] = endsym;
    wsym[8] = forsym;               //add
    wsym[9] = ifsym;
    wsym[10] = oddsym;
    wsym[11] = procsym;
    wsym[12] = readsym;
    wsym[13] = returnsym;           //add
    wsym[14] = realsym;             //add
    wsym[15] = thensym;
    wsym[16] = tosym;               //add
    wsym[17] = varsym;
    wsym[18] = whilesym;
    wsym[19] = writesym;

    /*设置指令名称*/
    strcpy(&(mnemonic[lit][0]), "lit");
    strcpy(&(mnemonic[opr][0]), "opr");
    strcpy(&(mnemonic[lod][0]), "lod");
    strcpy(&(mnemonic[sto][0]), "sto");
    strcpy(&(mnemonic[cal][0]), "cal");
    strcpy(&(mnemonic[inte][0]), "int");
    strcpy(&(mnemonic[jmp][0]), "jmp");
    strcpy(&(mnemonic[jpc][0]), "jpc");
    /*------------add_up---------------*/
    strcpy(&(mnemonic[sta][0]), "sta");
    strcpy(&(mnemonic[lda][0]), "lda");
    strcpy(&(mnemonic[arrcheck][0]), "arrcheck");
    /*-----------add_bottom------------*/

    /*设置符号集*/
    for (i = 0; i < symnum; i++)
    {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /*设置声明开始符号集*/
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[procsym] = true;
    /*---------add_up----------*/
    declbegsys[charsym] = true;     //增加声明开始符号charsym
    declbegsys[realsym] = true;     //增加声明开始符号realsym
    /*--------add_bottom--------*/

    /*设置语句开始符号集*/
    statbegsys[beginsym] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;

    /*设置因子开始符号集*/
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;
    /*--------add_up-----------*/
    facbegsys[dplus] = true;        //增加开始因子++
    facbegsys[dminus] = true;       //增加开始因子--
    facbegsys[charsym] = true;      //增加开始因子char
    /*--------add_bottom-------*/
}

/*
 *用数组实现集合的集合运算
 */
int inset(int e, bool* s)
{
    return s[e];
}
int addset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] || s2[i];
    }
    return 0;
}
int subset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] && (!s2[i]);
    }
    return 0;
}
int mulset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        sr[i] = s1[i] && s2[i];
    }
    return 0;
}

/*
 *出错处理，打印出错位置和错误编码
 */
void error(int n)

{
    char space[81];
    memset(space, 32, 81);
    space[cc - 1] = 0;//出错时当前符号已经读完，所以cc-1
    printf("****%s!%d\n", space, n);
    fprintf(fa1, "****%s!%d\n", space, n);
    err++;
}

/*
 *  漏掉空格，读取一个字符
 *
 *  每次读一行，存入line缓冲区，line被getsym取空后再读一行
 *
 *  被函数getsym调用
 */
int getch()
{
    if (cc == ll)
    {
        if (feof(fin))
        {
            printf("program incomplete");
            return -1;
        }
        ll = 0;
        cc = 0;
        printf("%d ", cx);
        fprintf(fa1, "%d ", cx);
        ch = ' ';
        while (ch != 10)
        {
            //fscanf(fin,"%c",&ch)
            if (EOF == fscanf(fin, "%c", &ch))
            {
                line[ll] = 0;
                break;
            }
            printf("%c", ch);
            fprintf(fa1, "%c", ch);
            line[ll] = ch;
            ll++;
        }
        printf("\n");
        fprintf(fa1, "\n");
    }
    ch = line[cc];
    cc++;
    return 0;
}


/*
*词法分析，获取一个符号
*/
int getsym()
{
    int i, j, k;
    while (ch == ' ' || ch == 10 || ch == 9)        /*忽略空格、换行和TAB*/
    {
        getchdo;
    }
    if (ch >= 'a' && ch <= 'z')
    {                   /*名字或保留字以a~z开头*/
        k = 0;
        do {
            if (k < al)
            {
                a[k] = ch;
                k++;
            }
            getchdo;
        } while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9');
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do {                /* 搜索当前符号是否为保留字 */
            k = (i + j) / 2;
            if (strcmp(id, word[k]) <= 0)
            {
                j = k - 1;
            }
            if (strcmp(id, word[k]) >= 0)
            {
                i = k + 1;
            }

        } while (i <= j);
        if (i - 1 > j)
        {
            sym = wsym[k];
        }
        else
        {
            sym = ident;     /* 搜索失败，则是名字或数字 */
        }
    }
    else
    {
        if (ch >= '0' && ch <= '9')     /* 检测是否为数字：以0~9开头 */
        {
            k = 0;
            num = 0;
            sym = number;
            do {
                num = 10 * num + ch - '0';
                k++;
                getchdo;
            } while (ch >= '0' && ch <= '9'); /*获取数字的值*/
            k--;
            if (k > nmax)
            {
                error(30);
            }
        }
        else
        {
            if (ch == ':')             /*检测赋值符号*/
            {
                getchdo;
                if (ch == '=')
                {
                    sym = becomes;
                    getchdo;
                }
                else
                {
                    //sym = nul;            /*不能识别的符号*/
                    sym = colon;            //冒号:
                }
            }
            else
            {
                if (ch == '<')         /*检测小于或小于等于符号*/
                {
                    getchdo;
                    if (ch == '=')
                    {
                        sym = leq;
                        getchdo;
                    }
                    /*--------add_up------------*/
                    else if (ch == '>')//小于号后面跟着大于号
                    {
                        sym = neq;//构成不等号<>
                        getchdo;
                    }
                    /*--------add_bottom------------*/
                    else
                    {
                        sym = lss;
                    }
                }
                else
                {
                    if (ch == '>')          /*检测大于或大于等于符号*/
                    {
                        getchdo;
                        if (ch == '=')
                        {
                            sym = geq;
                            getchdo;
                        }
                        else
                        {
                            sym = gtr;
                        }
                    }
                    /*---------add_up-------------*/
                    else if (ch == '+')
                    {
                        getchdo;
                        if (ch == '+')
                        {
                            sym = dplus;        //++    
                            printf("识别出++号\n");
                            getchdo;
                        }
                        else if (ch == '=')
                        {
                            sym = peql;     //+=
                            getchdo;
                        }
                        else
                        {
                            sym = plus;
                        }
                    }
                    else if (ch == '-')
                    {
                        getchdo;
                        if (ch == '-')
                        {
                            sym = dminus;       //--
                            printf("识别出--号\n");
                            getchdo;
                        }
                        else if (ch == '=')
                        {
                            sym = meql;            //-=
                            printf("识别出-=号\n");
                            getchdo;
                        }
                        else
                        {
                            sym = minus;
                        }
                    }
                    /*--------add_bottom-------------*/
                    
                    /*--------add_up-----------------*/
                    /* 增加字符型 */
                    else if (ch == '\'')
                    {
                        getchdo;
                        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                        {
                            num = (int)ch;
                            getchdo;
                            if (ch == '\'')
                                sym = charsym;
                            else
                            {
                                num = 0;
                                sym = nul;
                                error(39);  //类型错误                  
                            }
                        }
                        else error(39); //类型不匹配
                        getchdo;
                    }
                    /* 增加注释 */
                    /*在这个循环里面的条件判断是为了处理读完整个符号都没能遇到右括号的情况，
                    * 即发生括号不匹配的错误。
                    * 最后一个getsymdo是为了不影响之后程序的运行，当前注释没有意义，
                    * 所以再读入一个符号作为本次读入的结果。
                    */
                    else if (ch == '{') {
                        do {
                            if (cc == ll && ch != '}') {
                                error(31);      //没有匹配的右括号
                            }
                            getchdo;
                        } while (ch!='}');
                        getchdo;    //读入下一个符号前，必须先读入下一个字符
                        getsymdo;   //获取下一个符号，下一个符号为真实符号，当前'}'符号没有意义
                    }
                    /*-----------add_bottom-----------*/
                    else
                    {
                        sym = ssym[ch];/* 当符号不满足上述条件时，全部按照单字符号处理*/
                        //getchdo;
                        //richard
                        if (sym != period)
                        {
                            getchdo;
                        }
                        //end richard
                    }
                }
            }
        }
    }
    return 0;
}

/*
*生成虚拟机代码
*x:instruction.f;
*y:instruction.l;
*z:instruction.a;
*/
int gen(enum fct x, int y, int z)
{
    if (cx >= cxmax)
    {
        printf("Program too long"); /*程序过长*/
        return -1;
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
    return 0;
}

/*
*测试当前符号是否合法
*
*在某一部分（如一条语句，一个表达式）将要结束时时我们希望下一个符号属于某集合
*（该部分的后跟符号） test 负责这项检测，并且负责当检测不通过时的补救措施
*程序在需要检测时指定当前需要的符号集合和补救用的集合（如之前未完成部分的后跟
*符号），以及不通过时的错误号
*
*S1：我们需要的符号
*s2:如果不是我们需要的，则需要一个补救用的集合
*n:错误号
*/
int test(bool* s1, bool* s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);
        /*当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合*/
        while ((!inset(sym, s1)) && (!inset(sym, s2)))
        {
            getsymdo;
        }
    }
    return 0;
}

/*
 *编译程序主体
 *
 *lev:当前分程序所在层
 *tx:名字表当前尾指针
 *fsys:当前模块后跟符号集合
 */
int block(int lev, int tx, bool* fsys)
{
    int i;
    int dx;                         /*名字分配到的相对地址*/
    int tx0;                        /*保留初始tx*/
    int cx0;                        /*保留初始cx*/
    bool nxtlev[symnum];            /*在下级函数的参数中，符号集合均为值参，但由于使用数组
                                      实现，传递进来的是指针，为防止下级函数改变上级函数的
                                      集合，开辟新的空间传递给下级函数*/
    dx = 3;
    tx0 = tx;                         /*记录本层名字的初始位置*/
    table[tx].adr = cx;
    gendo(jmp, 0, 0);
    if (lev > levmax)
    {
        error(32);
    }
    do {
        if (sym == constsym)         /*收到常量声明符号，开始处理常量声明*/
        {
            getsymdo;
            do {
                constdeclarationdo(&tx, lev, &dx);   /*dx的值会被constdeclaration改变，使用
                                                     指针*/
                while (sym == comma)
                {
                    getsymdo;
                    constdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon)
                {
                    getsymdo;
                }
                else
                {
                    error(5); /*漏掉了逗号或者分号*/
                }
            } while (sym == ident);
        }
        if (sym == varsym)/*收到变量声名符号，开始处理变量声名*/
        {
            getsymdo;
            do {
                vardeclarationdo(&tx, lev, &dx);
                while (sym == comma)        //逗号,
                {
                    getsymdo;
                    vardeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon)       //分号;
                {
                    getsymdo;
                }
                else
                {
                    error(5);
                }
            } while (sym == ident);
        }
        /*-------------------add_up----------------*/
        if (sym == charsym)     /*收到符号型声名符号，开始处理符号型声名*/
        {
            getsymdo;
            do {
                chardeclarationdo(&tx, lev, &dx);
                while (sym == comma)        //逗号
                {
                    getsymdo;
                    chardeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon){       //分号
                    getsymdo;
                }
                else{
                    error(5);   /*漏掉了分号*/          
                }
            } while (sym == ident);
        }
        if (sym == realcon) {
            getsymdo;
            do {
                realdeclarationdo(&tx, lev, &dx);
                while (sym == comma) {
                    getsymdo;
                    realdeclarationdo(&tx, lev, &dx);
                }
                if (sym == semicolon) {
                    getsymdo;
                }
                else {
                    error(5);
                }
            } while (sym == ident);
        }
        /*-----------------add_bottom---------------*/
        while (sym == procsym)/*收到过程声名符号，开始处理过程声名*/
        {
            getsymdo;
            if (sym == ident)
            {
                enter(procedur, &tx, lev, &dx);/*记录过程名字*/
                getsymdo;
            }
            else
            {
                error(4);/*procedure后应为标识符*/
            }
            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);/*漏掉了分号*/
            }
            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            if (-1 == block(lev + 1, tx, nxtlev))
            {
                return -1;/*递归调用*/
            }
            if (sym == semicolon)
            {
                getsymdo;
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                testdo(nxtlev, fsys, 6);
            }
            else
            {
                error(5);                       /*漏掉了分号*/
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        nxtlev[period] = true;
        testdo(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys));                /*直到没有声明符号*/
    code[table[tx0].adr].a = cx;                    /*开始生成当前过程代码*/
    table[tx0].adr = cx;                            /*当前过程代码地址*/
    table[tx0].size = dx;                           /*声明部分中每增加一条声明都会给dx增加1,声明部分已经结束,dx就是当前过程数据的size*/

    cx0 = cx;
    gendo(inte, 0, dx);                             /*生成分配内存代码*/
    if (tableswitch)                               /*输出名字表*/
    {
        printf("TABLE:\n");
        if (tx0 + 1 > tx)
        {
            printf("NULL\n");
        }
        for (i = tx0 + 1; i <= tx; i++)
        {
            switch (table[i].kind)
            {
            case constant:
                printf("%d const %s ", i, table[i].name);
                printf("val=%d\n", table[i].val);
                fprintf(fas, "%d const %s ", i, table[i].name);
                fprintf(fas, "val=%d\n", table[i].val);
                break;
            case variable:
                printf("%d var %s ", i, table[i].name);
                printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
                fprintf(fas, "%d var %s ", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d\n", table[i].level, table[i].adr);
                break;
                /*-----------add_up-------------*/
                /* 输出字符类型 */
            case charcon:
                printf("%d char %s ", i, table[i].name);
                printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
                fprintf(fas, "%d char %s ", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d\n", table[i].level, table[i].adr);
                break;
                /*----------add_bottom----------*/
                 /*-----------add_up-------------*/
                /* 输出一维数组类型 */
            case array:
                printf("%d array %s ", i, table[i].name);
                printf("lev=%d addr=%d size=%d low=%d\n", table[i].level, table[i].adr, table[i].size, table[i].startid);
                fprintf(fas, "%d array %s ", i, table[i].name);
                fprintf(fas, "lev=%d addr=%d size=%d low=%d\n", table[i].level, table[i].adr, table[i].size, table[i].startid);
                break;
                /*----------add_bottom----------*/
            case procedur:
                printf("%d proc %s ", i, table[i].name);
                printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                fprintf(fas, "%d proc %s ", i, table[i].name);
                fprintf(fas, "lev=%d adr=%d size=%d \n", table[i].level, table[i].adr, table[i].size);
                break;
            }
        }
        printf("\n");
    }
    /*语句后跟符号为分号或end*/
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);/*每个后跟符号集和都包含上层后跟符号集和，以便补救*/
    nxtlev[semicolon] = true;
    nxtlev[endsym] = true;
    statementdo(nxtlev, &tx, lev);
    gendo(opr, 0, 0); /*每个过程出口都要使用的释放数据段命令*/
    memset(nxtlev, 0, sizeof(bool) * symnum); /*分程序没有补救集合*/
    test(fsys, nxtlev, 8);                  /*检测后跟符号正确性*/
    listcode(cx0);                        /*输出代码*/
    return 0;
}

/*
*在名字表中加入一项
*
*k:名字种类const,var or procedure
*ptx:名字表尾指针的指针，为了可以改变名字表尾指针的数值
*lev:名字所在的层次，以后所有的lev都是这样
*pdx:为当前应分配的变量的相对地址，分配后要增加1
*/
void enter(enum object k, int* ptx, int lev, int* pdx)
{
    (*ptx)++;                           
    strcpy(table[(*ptx)].name, id);       /*全局变量id中已存有当前名字的名字*/
    table[(*ptx)].kind = k;
    switch (k)
    {
    case constant:                      /*常量名字*/
        if (num > amax)
        {
            error(31);
            num = 0;
        }
        table[(*ptx)].val = num;
        break;
    case variable:                     /*变量名字*/
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;                          /*过程名字*/
    case procedur:
        table[(*ptx)].level = lev;
        break;
        /*----------------add_up----------*/
    case charcon:                       /*字符型名字*/
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    case realcon:                       /*实型名字*/
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*ptx);
        (*ptx)++;
        break;
        /*----------------add_bottom----------*/
        /*----------------add_up----------*/
    case array:
        table[(*ptx)].level = lev;
        table[(*ptx)].adr = (*pdx);
        break;
        /*----------------add_bottom----------*/
    }

}

/*
 *查找名字的位置
 *找到则返回在名字表中的位置，否则返回0
 *
 *idt: 要查找的名字
 *tx:：当前名字表尾指针
 */
int position(char* idt, int  tx)
{
    int i;
    strcpy(table[0].name, idt);
    i = tx;
    while (strcmp(table[i].name, idt) != 0)
    {
        i--;
    }
    return i;
}

/*
 *常量声明处理
 */
int constdeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        getsymdo;
        if (sym == eql || sym == becomes)
        {
            if (sym == becomes)
            {
                error(1);                     /*把=写出成了：=*/
            }
            getsymdo;
            if (sym == number)
            {
                enter(constant, ptx, lev, pdx);
                getsymdo;
            }
            else
            {
                error(2);                  /*常量说明=后应是数字*/
            }
        }
        else
        {
            error(3);                       /*常量说明标识后应是=*/
        }
    }
    else
    {
        error(4);                        /*const后应是标识*/
    }
    return 0;
}

/*
 *变量声明处理
 *增加一维数组声明
 */
int vardeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        // 填写名字表并改变堆栈帧计数器
        int startid = 0, endid = 0;
        enter(variable, ptx, lev, pdx);         /*填写名字表，无论是变量还是数组
                                                * 都可以先进行登记，
                                                * 随后正对数组进行修改参数*/
        (*pdx)++;
        getsymdo;
        if (sym == lparen) {
            getsymdo;
            switch (sym) {
            case ident:
                startid = table[position(id, *ptx)].val;
                break;
            case number:
                startid = num;
                break;
            case plus:
                getsymdo;
                startid = num;
                break;
            case minus:
                getsymdo;
                startid = -num;
                break;
            }
            printf("分配前table[(*ptx)].adr值= %d\n", table[(*ptx)].adr);
            table[(*ptx)].adr = table[(*ptx)].adr - startid;    //数组的地址减去一个下界值偏移量
            table[(*ptx)].startid = startid;                    //记录数组的下界                 
            getsymdo;
            if (sym != colon) {
                error(30);
            }
            else {
                getsymdo;
                switch (sym) {
                case ident:
                    endid = table[position(id, *ptx)].val;
                    table[(*ptx)].size = endid - startid + 1;
                    break;
                case number:
                    endid = num ;
                    table[(*ptx)].size = endid - startid + 1;
                    break;
                case plus:
                    getsymdo;
                    endid = num ;
                    table[(*ptx)].size = endid - startid + 1;
                    break;
                case minus:
                    getsymdo;
                    endid = -num ;
                    table[(*ptx)].size = endid - startid + 1;
                    break;
                }
                table[(*ptx)].kind = array;
                (*pdx) = (*pdx) + endid - startid + 1;	//分配连续区间
                getsymdo;
                getsymdo;
            }
        }
    }
    else
    {
        error(4);
    }
    return 0;
}

/*----------------------add_up---------------------*/
/*
 *字符型声明
 */
int chardeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        enter(charcon, ptx, lev, pdx);//填写名字表
        getsymdo;
    }
    else
    {
        error(4);
    }
    return 0;
}

/*
 *实型声明
 */
 int realdeclaration(int * ptx,int lev,int * pdx)
 {
	 if(sym==ident)
	 {
		 enter(realcon,ptx,lev,pdx);//填写名字表
		 getsymdo;
	 }
	 else
	 {
		 error(4);
	 }
	 return 0;
 }
 
 /*---------------------add_bottom---------------------*/


 /*
  *输入目标代码清单
  */
void listcode(int cx0)
{
    int i;
    if (listswitch)
    {
        for (i = cx0; i < cx; i++)
        {
            printf(" %d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
            fprintf(fa, " %d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}
/*
*语句处理
*/
int statement(bool* fsys, int* ptx, int lev)
{
    int i, cx1, cx2;
    bool nxtlev[symnum];
    if (sym == ident)       /*准备按照赋值语句处理*/
    {
        i = position(id, *ptx); //在符号表中查到该标识符所在位置
        if (i == 0)
        {
            error(11);      /*变量未找到*/
        }
        else
        {
            if ((table[i].kind != variable) && (table[i].kind != charcon)&&(table[i].kind!=array))  //添加了字符类型和一维数组类型
            {
                error(12);
                i = 0;
            }
            /*---------------------add_up-----------------*/
            /* 添加字符类型 */
            else if (table[i].kind == charcon)
            {
                getsymdo;
                if (sym != becomes) error(13);
                else
                {
                    getsymdo;
                    if (sym != charsym) error(32);
                    else
                    {
                        gendo(lit, 0, num);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }

                }
                getsymdo;   //声明后面有分号
            }
            /*-------------------add_bottom---------------*/
            else
            {
                getsymdo;
                if (sym == becomes)
                {
                    getsymdo;
                    /*------------------add_up----------------*/
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    if (i != 0)         //i应指为当前语句左部标识符在符号表中的位置 
                    {
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                    /*------------------add_bottom------------*/
                }
                /*-------------------------add_up-----------------------*/
                else if (sym == lparen) {		// 处理一维数组赋值，赋值格式可以是 arrayName(1+3):=10，以此为例
                    getsymdo;   //获取到第一个num=1
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);    //计算1+3
                    expressiondo(nxtlev, ptx, lev);                 //将值置于栈顶
                    getsymdo;	                                    //获取到下一个符号‘)’
                    getsymdo;                                       //获取赋值符号 ‘:=’
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);    //计算右侧表达式
                    expressiondo(nxtlev, ptx, lev);       
                    gendo(sta, lev - table[i].level, table[i].adr); //调用sta指令完成赋值
                }
                /*-----------------------add_bottom---------------------*/
                /*--------------add_up----------------*/
                else if (sym == peql)    // +=
                {
                    i = position(id, *ptx);     //取变量所在符号表位置
                    gendo(lod, lev - table[i].level, table[i].adr);      /* 找到变量地址并将其值入栈 */
                    getsymdo;
                    if (sym == semicolon)
                    {
                        getsymdo;
                    }
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    gendo(opr, 0, 2);
                    if (i != 0)
                    {
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == meql)   //  -=
                {
                    i = position(id, *ptx);     //取变量所在符号表位置
                    gendo(lod, lev - table[i].level, table[i].adr);      /* 找到变量地址并将其值入栈 */
                    getsymdo;
                    if (sym == semicolon)
                    {
                        getsymdo;
                    }
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    expressiondo(nxtlev, ptx, lev);
                    gendo(opr, 0, 3);
                    if (i != 0)
                    {
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == dplus) // 后++运算 
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr); //找到变量地址，将其值入栈
                    gendo(lit, 0, 1);   //将常数1取到栈顶
                    if (i != 0)
                    {
                        gendo(opr, 0, 2);     //执行加操作
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                else if (sym == dminus) // 后--运算 
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr);//找到变量地址，将其值入栈
                    gendo(lit, 0, 1);//将常数1取到栈顶
                    if (i != 0)
                    {
                        gendo(opr, 0, 3);     //执行减操作
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }
                /*-------------------add_bottom---------------*/
                else
                {
                    error(13);
                }

            }
        }
    }
    /*---------------add_up------------*/
    else if (sym == dplus) // 前++运算 
    {
        getsymdo;
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0) error(11);
            else
            {
                if (table[i].kind != variable)
                {
                    error(12);
                    i = 0;
                }
                else    //++后跟的是变量
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr);//找到变量地址，将其值入栈
                    gendo(lit, 0, 1);//将常数1取到栈顶
                    if (i != 0)
                    {
                        gendo(opr, 0, 2);     //执行加操作
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }

            }
        }
        else
        {
            error(19);
        }
    }
    else if (sym == dminus) // 前--运算 
    {
        getsymdo;
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0) error(11);
            else
            {
                if (table[i].kind != variable)
                {
                    error(12);
                    i = 0;
                }
                else  //--后跟的是变量
                {
                    getsymdo;
                    gendo(lod, lev - table[i].level, table[i].adr);//找到变量地址，将其值入栈
                    gendo(lit, 0, 1);//将常数1取到栈顶
                    if (i != 0)
                    {
                        gendo(opr, 0, 3);     //执行减操作
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                }

            }
        }
        else
        {
            error(19);
        }
    }
    /*------------------add_bottom--------------*/
    /*------------------add_up------------------*/
    else if (sym == forsym)      //检测到for语句
    {

        getsymdo;
        if (sym == ident)
        {
            i = position(id, *ptx);
            if (i == 0) error(11);
            else
            {
                if (table[i].kind != variable) //赋值语句中，赋值号左部标识符属性应是变量
                {
                    error(12); i = 0;
                }
                else
                {
                    getsymdo;
                    if (sym != becomes) error(13);            //后跟符号应该是赋值符号 ：=
                    else getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[tosym] = true;                     //后跟符to和downto
                    nxtlev[downtosym] = true;
                    expressiondo(nxtlev, ptx, lev);           //计算表达式1值
                    gendo(sto, lev - table[i].level, table[i].adr);     //值存回地址
                    switch (sym)
                    {
                    case tosym:                             //+2
                        getsymdo;
                        cx1 = cx;                           //循环体结尾需跳回开始地址
                        gendo(lod, lev - table[i].level, table[i].adr);
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);        //处理表达式2，提供空间
                        nxtlev[dosym] = true;                               //后跟符do
                        expressiondo(nxtlev, ptx, lev);                 //计算表达式2值
                        gendo(opr, 0, 13);                  //生成比较指令，i是否小于等于E2的值
                        cx2 = cx;                           //保存循环体结束的下一位置
                        //生成条件跳转指令，跳出循环，跳出的地址需回填
                        gendo(jpc, 0, 0);
                        if (sym == dosym)               //处理循环体S
                        {
                            getsymdo;
                            statement(fsys, ptx, lev);  //循环体处理
                            //增加循环变量步长为2 
                            gendo(lod, lev - table[i].level, table[i].adr);
                            gendo(lit, 0, 2);                          
                            gendo(opr, 0, 2);                          
                            gendo(sto, lev - table[i].level, table[i].adr);
                            gendo(jmp, 0, cx1);                 //无条件跳转到循环开始点
                            code[cx2].a = cx;                   //回填跳出循环的地址，与while类似
                        }
                        else
                        {
                            error(29);    //for语句中少了do
                        }
                        break;

                    case downtosym:            //步长为的向下减少
                        getsymdo;
                        cx1 = cx;            //保存循环开始点
                        //将循环判断变量取出放到栈顶                         
                        gendo(lod, lev - table[i].level, table[i].adr);
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);      //处理表达式E2
                        nxtlev[dosym] = true;                           //后跟符do
                        expressiondo(nxtlev, ptx, lev);
                        /*判断循环变量条件，比如for i:=E1 downto E2 do S中，判断i是否大于等于E2，
                        如大于等于，继续循环， 小于的话，跳出循环*/
                        gendo(opr, 0, 11);   //生成比较指令，i是否大于等于E2的值
                        cx2 = cx;            //保存循环结束点
                        //生成条件跳转指令，跳出循环，跳出的地址未知             
                        gendo(jpc, 0, 0);
                        if (sym == dosym)     //处理循环体S
                        {
                            getsymdo;
                            statement(fsys, ptx, lev);   //循环体处理
                            //增加循环变量步长为2 
                            //将循环变量取出放在栈顶
                            gendo(lod, lev - table[i].level, table[i].adr);
                            gendo(lit, 0, 2);                 //将步长取到栈顶
                            gendo(opr, 0, 3);                 //循环变量加步长
                            //将栈顶的值存入循环变量
                            gendo(sto, lev - table[i].level, table[i].adr);
                            gendo(jmp, 0, cx1);     //无条件跳转到循环开始点
                            /*回填循环结束点的地址，cx为else后语句执行完的位置，它正是前面未定的跳转地址*/
                            code[cx2].a = cx;
                        }
                        else
                        {
                            error(29);//for语句中少了do
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            error(19);         //for语句后跟赋值语句，赋值语句左部是变量，缺少变量
        }
    }
    /*-----------------add_bottom-------------*/
    else
    {
        if (sym == readsym)
        {
            getsymdo;
            if (sym != lparen)
            {
                error(34);
            }
            else
            {
                do {
                    getsymdo;
                    if (sym == ident)
                    {
                        i = position(id, *ptx);
                    }
                    else
                    {
                        i = 0;
                    }
                    if (i == 0)
                    {
                        error(35);
                    }
                    /*------------add_up-------------*/
                    /* 字符型数输入 */
                    else if (table[i].kind == charcon)//字符型数输入
                    {
                        gendo(opr, 0, 19);
                        gendo(sto, lev - table[i].level, table[i].adr);
                    }
                    /*-------------add_bottom------------*/

                    /*--------------add_up-------------*/
                    /* 一维数组输出 */
                    else if (table[i].kind == array) {
                        getsymdo;
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        expressiondo(nxtlev, ptx, lev);
                        gendo(arrcheck, table[i].startid, table[i].size);
                        gendo(jpc, 0, 0);
                        gendo(opr, 0, 16);
                        gendo(sta, lev - table[i].level, table[i].adr);
                    }
                    /*-----------add_bottom------------*/
                    else
                    {
                        gendo(opr, 0, 16);
                        gendo(sto, lev - table[i].level, table[i].adr); /* 储存到变量*/
                    }
                    getsymdo;
                } while (sym == comma);    /*一条read语句可读多个变量 */
            }
            if (sym != rparen)
            {
                error(33);          /* 格式错误，应是右括号*/
                while (!inset(sym, fsys))/* 出错补救，直到收到上层函数的后跟符号*/
                {
                    getsymdo;
                }
            }
            else
            {
                getsymdo;
            }
        }
        else
        {
            if (sym == writesym)           /* 准备按照write语句处理，与read类似*/
            {
                getsymdo;
                if (sym == lparen)
                {
                    do {
                        getsymdo;
                        if (sym == ident)
                        {
                            i = position(id, *ptx);
                            if (i == 0)
                            {
                                error(11);          /*过程未找到*/
                            }
                        }
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        nxtlev[comma] = true;     /* write的后跟符号为）or，*/
                        expressiondo(nxtlev, ptx, lev);/* 调用表达式处理，此处与read不同，read为给变量赋值*/
                        /*---------------add_up--------------*/
                        if (table[i].kind == charcon) //字符型输出 
                        {
                            gendo(opr, 0, 17);
                        }
                        /*---------------add_bottom----------*/
                        else gendo(opr, 0, 14);/* 生成输出指令，输出栈顶的值*/
                    } while (sym == comma);
                    if (sym != rparen)
                    {
                        error(33);/* write()应为完整表达式*/
                    }
                    else
                    {
                        getsymdo;
                    }
                }
                gendo(opr, 0, 15);        /* 输出换行*/
            }
            else
            {
                if (sym == callsym)        /* 准备按照call语句处理*/
                {
                    getsymdo;
                    if (sym != ident)
                    {
                        error(14);           /*call后应为标识符*/
                    }
                    else
                    {
                        i = position(id, *ptx);
                        if (i == 0)
                        {
                            error(11);          /*过程未找到*/
                        }
                        else
                        {
                            if (table[i].kind == procedur)
                            {
                                gendo(cal, lev - table[i].level, table[i].adr);  /*生成call指令*/
                            }
                            else
                            {
                                error(15);      /*call后标识符应为过程*/
                            }
                        }
                        getsymdo;
                    }
                }
                else
                {
                    if (sym == ifsym)
                    {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;
                        /*-------add_up-------*/
                        nxtlev[elsesym] = true;//添加后跟符号else
                        /*-------add_bottom------*/
                        conditiondo(nxtlev, ptx, lev);
                        if (sym == thensym)
                        {
                            getsymdo;
                        }
                        else
                        {
                            error(16);
                        }
                        cx1 = cx;
                        gendo(jpc, 0, 0);
                        statementdo(fsys, ptx, lev);
                        code[cx1].a = cx;

                        /*-------add_up---------*/
                        if (sym != elsesym) { code[cx1].a = cx; }
                        else
                        {
                            getsymdo;
                            cx2 = cx;
                            code[cx1].a = cx + 1;
                            gendo(jmp, 0, 0);
                            statementdo(fsys, ptx, lev);
                            code[cx2].a = cx;
                        }
                        /*------add_bottom--------*/
                    }
                    else
                    {
                        if (sym == beginsym)   /*准备按照复合语句处理*/
                        {
                            getsymdo;
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true;/*后跟符号为分号或end*/
                                                    /*循环调用语句处理函数，直到下一个符号不是语句开始符号或收到end*/
                            statementdo(nxtlev, ptx, lev);
                            while (inset(sym, statbegsys) || sym == semicolon)
                            {
                                if (sym == semicolon)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(10);/*缺少分号*/
                                }
                                statementdo(nxtlev, ptx, lev);
                            }
                            if (sym == endsym)
                            {
                                getsymdo;
                            }
                            else
                            {
                                error(17); /*缺少end或分号*/
                            }
                        }
                        else
                        {
                            if (sym == whilesym)/*准备按照while语句处理*/
                            {
                                cx1 = cx;        /*保存判断条件超作的位置*/
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[dosym] = true;/*后跟符号为do*/
                                conditiondo(nxtlev, ptx, lev);  /*调用条件处理*/
                                cx2 = cx;       /*保存循环体的结束的下一个位置*/
                                gendo(jpc, 0, 0);/*生成条件跳转，但跳出循环的地址未知*/
                                if (sym == dosym)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(18);      /*缺少do*/
                                }
                                statementdo(fsys, ptx, lev); /*循环体*/
                                gendo(jmp, 0, cx1);/*回头重新判断条件*/
                                code[cx2].a = cx;   /*反填跳出循环的地址，与if类似*/
                            }
                            else
                            {
                                memset(nxtlev, 0, sizeof(bool) * symnum);/*语句结束无补救集合*/
                                testdo(fsys, nxtlev, 19);/*检测语句结束的正确性*/
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//括号对应有问题,需解决？？，已经解决，原780行少了个括号

/*
*表达式处理
*/
int expression(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;                    /*用于保存正负号*/
    bool nxtlev[symnum];
    if (sym == plus || sym == minus)             /*开头的正负号，此时当前表达式被看作一个正的或负的项*/
    {
        addop = sym;                    /*保存开头的正负号*/
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);                /*处理项*/
        if (addop == minus)
        {
            gendo(opr, 0, 1);                   /*如果开头为负号生成取负指令*/
        }
    }
    else                             /*此时表达式被看作项的加减*/
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);            /*处理项*/
    }
    while (sym == plus || sym == minus)
    {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);              /*处理项*/
        if (addop == plus)
        {
            gendo(opr, 0, 2);                    /*生成加法指令*/
        }
        else
        {
            gendo(opr, 0, 3);                /*生成减法指令*/
        }
    }
    return 0;
}

/*
*项处理
*/
int term(bool* fsys, int* ptx, int lev)
{
    enum symbol mulop;               /*用于保存乘除法符号*/
    bool nxtlev[symnum];
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    factordo(nxtlev, ptx, lev);       /*处理因子*/
    while (sym == times || sym == slash)
    {
        mulop = sym;
        getsymdo;
        factordo(nxtlev, ptx, lev);
        if (mulop == times)
        {
            gendo(opr, 0, 4);          /*生成乘法指令*/
        }
        else
        {
            gendo(opr, 0, 5);           /*生成除法指令*/
        }
    }
    return 0;
}

/*
*因子处理
*/
int factor(bool* fsys, int* ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys, fsys, 24);           /*检测因子的开始符好号*/
    while (inset(sym, facbegsys))          /*循环直到不是因子开始符号*/
    {
        if (sym == ident)                   /*因子为常量或者变量*/
        {
            i = position(id, *ptx);        /*查找名字*/
            if (i == 0)
            {
                error(11);               /*标识符未声明*/
            }
            else
            {
                switch (table[i].kind)
                {
                case constant:                                      /*名字为常量*/
                    gendo(lit, 0, table[i].val);                       /*直接把常量的值入栈*/
                    break;
                case variable:                                      /*名字为变量*/
                    gendo(lod, lev - table[i].level, table[i].adr);      /*找到变量地址并将其值入栈*/
                    break;
                case procedur:                                      /*名字为过程*/
                    error(21);                                       /*不能为过程*/
                    break;
                     /*------------add_up--------------*/
                case charcon:                                      /*名字为字符型*/
                    gendo(lod, lev - table[i].level, table[i].adr);      /*找到字符型地址并将其值入栈*/
                    break;
                      /*-----------add_bottom-----------*/
                    /*------------add_up--------------*/
                case array:                                         /*一维数组类型*/
                    getsymdo;
                    if (sym == lparen)
                    {					//是数组
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        expressiondo(nxtlev, ptx, lev);
                        gendo(lda, lev - table[i].level, table[i].adr);
                        //getsymdo;							
                    }
                    if (sym == rparen)
                    {
                        //getsymdo;
                    }
                    /*-----------add_bottom-----------*/
                }
            }
            getsymdo;
            /*----------------add_up---------------*/
            if (sym == dplus)
            {
                /* 此处是A++运算，将值写回A */
                gendo(lit, lev - table[i].level, 1);    //取值入栈
                gendo(opr, lev - table[i].level, 2);    //+1,栈顶加次栈顶
                gendo(sto, lev - table[i].level, table[i].adr);//存值
                gendo(lod, lev - table[i].level, table[i].adr);//取值到栈顶
                getsymdo;
            }
            else if (sym == dminus)
            {
                gendo(lit, lev - table[i].level, 1);//将值为入栈
                gendo(opr, lev - table[i].level, 3);//-1,栈顶减次栈顶
                gendo(sto, lev - table[i].level, table[i].adr);//出栈取值到内存
                gendo(lod, lev - table[i].level, table[i].adr);//取值到栈顶
                getsymdo;
            }
        }
        else if (sym == dplus)
        {
            getsymdo;
            if (sym == ident)
            {
                getsymdo;
                i = position(id, *ptx);
                if (i == 0) error(11);
                else
                {
                    if (table[i].kind == variable)
                    {
                        gendo(lod, lev - table[i].level, table[i].adr);//找到变量地址，将其值入栈
                        gendo(lit, 0, 1);//将常数1取到栈顶
                        gendo(opr, 0, 2);     //执行加操作
                        gendo(sto, lev - table[i].level, table[i].adr);//出栈取值到内存
                        gendo(lod, lev - table[i].level, table[i].adr);//取值到栈顶
                    }
                }
            }
        }
        else if (sym == dminus)
        {
            getsymdo;
            if (sym == ident)
            {
                getsymdo;
                i = position(id, *ptx);
                if (i == 0) error(11);
                else
                {
                    if (table[i].kind == variable)
                    {
                        gendo(lod, lev - table[i].level, table[i].adr);//找到变量地址，将其值入栈
                        gendo(lit, 0, 1);//将常数1取到栈顶
                        gendo(opr, 0, 3);     //执行减操作
                        gendo(sto, lev - table[i].level, table[i].adr);//出栈取值到内存
                        gendo(lod, lev - table[i].level, table[i].adr);//取值到栈顶
                    }
                }
            }
        }
        /*-------------------add_bottom--------------*/
        else
        {
            if (sym == number)                                             /*因子为数*/
            {
                if (num > amax)
                {
                    error(31);
                    num = 0;
                }
                gendo(lit, 0, num);
                getsymdo;
            }
            else
            {
                if (sym == lparen)                                           /*因子为表达式*/
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expressiondo(nxtlev, ptx, lev);
                    if (sym == rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22);                                       /*缺少右括号*/
                    }
                }
                testdo(fsys, facbegsys, 23);                        /*银子后有非法符号*/
            }
        }
    }
    return 0;
}

/*
 条件处理*/
int condition(bool* fsys, int* ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];
    if (sym == oddsym)                        /*准备按照odd运算处理*/
    {
        getsymdo;
        expressiondo(fsys, ptx, lev);
        gendo(opr, 0, 6);                              /*生成odd指令*/
    }
    else
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;
        expressiondo(nxtlev, ptx, lev);
        if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
        {
            error(20);
        }
        else
        {
            relop = sym;
            getsymdo;
            expressiondo(fsys, ptx, lev);
            switch (relop)
            {
            case eql:
                gendo(opr, 0, 8);
                break;
            case neq:
                gendo(opr, 0, 9);
                break;
            case lss:
                gendo(opr, 0, 10);
                break;
            case geq:
                gendo(opr, 0, 11);
                break;
            case gtr:
                gendo(opr, 0, 12);
                break;
            case leq:
                gendo(opr, 0, 13);
                break;
            }

        }
    }
    return 0;
}    

/*
    *解释程序
    */
void interpret()
{
    int p, b, t;             /*指令指针，指令基址，栈顶指针*/
    struct instruction i;  /*存放当前指令*/
    int s[stacksize];      /*栈*/
    /*---------add_up------------*/
    //double s[stacksize];  //改成doubule型栈
    /*---------add_bottom--------*/
    printf("\nstart pl0\n");
    t = 0;
    b = 0;
    p = 0;
    s[0] = s[1] = s[2] = 0;
    do {
        i = code[p];         /*读当前指令*/
        p++;
        switch (i.f)
        {
        case lit:        /*将a的值取到栈顶*/
            s[t] = i.a;
            t++;
            break;
        case opr:        /*数字、逻辑运算*/
            switch (i.a)
            {
            case 0:
                t = b;
                p = s[t + 2];
                b = s[t + 1];
                break;

            case 1:
                s[t - 1] = -s[t - 1];
                break;

            case 2:
                t--;
                s[t - 1] = s[t - 1] + s[t];
                break;

            case 3:
                t--;
                s[t - 1] = s[t - 1] - s[t];
                break;

            case 4:
                t--;
                s[t - 1] = s[t - 1] * s[t];
                break;

            case 5:
                t--;
                s[t - 1] = s[t - 1] / s[t];
                break;

            case 6:
                s[t - 1] = s[t - 1] % 2;
                break;

            case 8:
                t--;
                s[t - 1] = (s[t - 1] == s[t]);
                break;

            case 9:
                t--;
                s[t - 1] = (s[t - 1] != s[t]);
                break;

            case 10:
                t--;
                s[t - 1] = (s[t - 1] < s[t]);
                break;

            case 11:
                t--;
                s[t - 1] = (s[t - 1] >= s[t]);
                break;

            case 12:
                t--;
                s[t - 1] = (s[t - 1] > s[t]);
                break;

            case 13:
                t--;
                s[t - 1] = (s[t - 1] <= s[t]);
                break;

            case 14:        //14号操作为输出栈顶值操作
                printf("%d", s[t - 1]);     //未修改成实型字符型型前
                fprintf(fa2, "%d", s[t - 1]);
                t--;//栈顶下移
                break;

            case 15:    //15号操作为输出换行操作
                printf("\n");
                fprintf(fa2, "\n");
                break;

            case 16:
                printf("?");
                fprintf(fa2, "?");
                scanf("%d", &(s[t]));
                fprintf(fa2, "%d\n", s[t]);
                t++;
                break;

            /*----------------add_up---------------*/
            case 17:    //17号操作为输出栈顶值操作
                printf("输出字符:");
                printf("%c\n", s[t - 1]);//输出栈顶值
                fprintf(fa2, "%c\n", s[t - 1]);//同时打印到文件
                t--;//栈顶下移
                break;

                /*case 18://18号操作为输出栈顶值操作
                    printf("%lf",s[t-1]);//输出栈顶值
                    fprintf(fa2,"%lf\n",s[t-1]);//同时打印到文件
                    t--;//栈顶下移?
                    break;*/

            case 19://19号操作是接受键盘值输入到栈顶
                printf("输入单字符:");//屏显问号
                fprintf(fa2, "输入单字符:");//同时输出到文件
                getchar();//消除输入的enter
                scanf("%c", &(s[t]));

                //scanf("%s",sta);
                //s[t]=sta[0];//获得输入

                fprintf(fa2, "%c\n", s[t]);//把用户输入值打印到文件
                t++;//栈顶上移，分配空间
                break;

                /*case 20://20号操作是接受键盘值输入到栈顶
                    printf("输入双精度型数：");//屏显问号
                    fprintf(fa2,"输入双精度型数：");//同时输出到文件
                    scanf("%lf",&(s[t]));//获得输入
                    fprintf(fa2,"%lf\n",s[t]);//把用户输入值打印到文件
                    t++;//栈顶上移，分配空间
                    break;*/
            }
            break;
            /*----------------add_bottom------------*/

        case lod:       /*取相对当前过程的数据基地址为ａ的内存的值到栈顶*/
            s[t] = s[base(i.l, s, b) + i.a];
            t++;
            break;

        case sto:       /*栈顶的值存到相对当前过程的数据基地址为ａ的内存*/
            t--;
            s[base(i.l, s, b) + i.a] = s[t];
            break;

        case cal:              /*调用子程序*/
            s[t] = base(i.l, s, b); /*将父过程基地址入栈*/
            s[t + 1] = b;           /*将本过程基地址入栈，此两项用于base函数*/
            s[t + 2] = p;           /*将当前指令指针入栈*/
            b = t;                /*改变基地址指针值为新过程的基地址*/
            p = i.a;              /*跳转*/
            break;

        case inte:             /*分配内存*/
            t += i.a;
            break;

        case jmp:             /*直接跳转*/
            p = i.a;
            break;

        case jpc:              /*条件跳转*/     //jpc和jmp是有区别的！
            t--;
            if (s[t] == 0)
            {
                p = i.a;
            }
            break;
            /*----------------------------add_up--------------------------*/
        case sta:                        // 栈顶的值存到数组下标为a的内存
            t--;
            s[base(i.l, s, b) + i.a + s[t - 1]] = s[t];
            t--;
            break;
        case lda:
            s[t - 1] = s[base(i.l, s, b) + i.a + s[t - 1]];       //对数组的读取 第二个S[T]是放入偏移地址的				
            break;
        case arrcheck:
            s[t] = i.a;
            if ((s[t - 1] < i.l) || (s[t - 1] > s[t] + i.l - 1))
            {
                printf("Error:Out of the array's size!\n");
                fprintf(fa2, "Error:Out of the array's size!\n");
                s[t] = 0;
            }
            else {
                s[t] = 1;
            }
            t++;
            break;
            /*-----------------------------add_bottom------------------------------*/
        }
    } while (p != 0);
}

/*通过过程基址求上1层过程的基址*/
int base(int l, int* s, int b)
{
    int b1;
    b1 = b;
    while (l > 0)
    {
        b1 = s[b1];
        l--;
    }
    return b1;
}
