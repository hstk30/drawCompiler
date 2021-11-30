//
//  Scanner.c
//  CompilerDraw
//
//  Created by apple on 2016/12/12.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "Scanner.h"
#include <ctype.h>
#include <string.h>
FILE *fp = NULL;
char peek = ' ';
int line = 1;
char tmp[20];
int n = 0;

//int testnum = 0;


static struct Token token = {ERRTOKEN, "none", 0.0, NULL}; //如果外部文件不用它，就设为静态的！！

void readch(void)
{
    peek = getc(fp);
}

int readNextIs(char c)
{
    readch();
    if(peek != c)
        return 0;
    peek = ' ';
    return 1;
}

int initScan(char *fileDesc)
{
    fp = fopen(fileDesc, "r");
    if(fp == NULL)
    {
        printf("Can't open file:%s", fileDesc);
        return 0;
    }
    
    return 1;
}

//void test(void)
//{
//    while(testnum != 5 && (peek = getc(fp)) != EOF)
//    {
//        putc(peek, stdout);
//        testnum ++;
//    }
//    testnum ++;
//
//}

struct Token Scan(void){
    
    for(; peek != EOF; readch()){
        if(peek == ' ' || peek == '\t')
            continue;
        else if(peek == '\n'){
            line ++;
            //printf("On line:%d\n", line);
        }
        else
            break;
    }
    
    if(isdigit(peek))                                           //识别数字
    {
        n = 0;
        int v = 0;
        do{
            
            tmp[n] = peek;
            v = 10 * v + (peek - '0');                          //将字符转换为数字！
            readch();
            n++;
        }while(isdigit(peek));
        
        if(peek != '.')                                         //识别浮点数
        {
            tmp[n] = '\0';
            return (struct Token){CONST_ID, tmp, v, NULL};
        }
        
        double x = (double)v, base = 10.0;
        
        tmp[n] = peek; n++;
        for(;;){
            readch();
            if(!isdigit(peek))
                break;
            tmp[n] = peek;
            n++;
            x = x + (peek - '0') / base;
            base *= 10;
        }
        tmp[n] = '\0';
        return (struct Token){CONST_ID, tmp, x, NULL};
    }
    if(isalpha(peek))      //识别标识符
    {
        n = 0;
        do{
            
            tmp[n] = peek;
            readch();
            n ++;
        }while(isalnum(peek));
        tmp[n] = '\0';
        
        for(int i = 0; i < 18; i++)                             //如果不在字典中则返回ERRTOKRN
            if(strcmp(tmp, TokenTab[i].lexeme) == 0)
            {
                return TokenTab[i];
            }
        
        token.lexeme = tmp;
        return token;
    }
    
    switch(peek)                                                //识别分隔符和运算符
    {                                              //匹配之后 还是要取得下一个字符才可以退出，不然陷入循环
        case ';':
            readch();
            return (struct Token){SEMICO, ";", 0.0, NULL};
        case ',':
            readch();
            return (struct Token){COMMA, ",", 0.0, NULL};
        case '*':
            if(readNextIs('*'))
                return (struct Token){POWER, "**", 0.0, NULL};
            else
                return (struct Token){MUL, "*", 0.0, NULL};
        case '/':
            readch();
            return (struct Token){DIV, "/", 0.0, NULL};
        case '-':
            readch();
            return (struct Token){MINUS, "-", 0.0, NULL};
        case '(':
            readch();
            return (struct Token){L_BRACKET, "(", 0.0, NULL};
        case ')':
            readch();
            return (struct Token){R_BRACKET, ")", 0.0, NULL};
        case '+':
            readch();
            return (struct Token){PLUS, "+", 0.0, NULL};
        default:
            break;
    }
    
    token.type = NONTOKEN;                                      //到达文件结尾，返回空标记
    return token;
}

int closeScan(void)
{
    if(fclose(fp) != 0)
    {
        printf("close fail!\n");
        return 0;
    }
    return 1;
}
