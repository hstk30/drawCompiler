//
//  Token.h
//  CompilerDraw
//
//  Created by apple on 2016/12/12.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef Token_h
#define Token_h
#include <stdio.h>

enum Token_Type{
    ORIGIN, SCALE, ROT, IS,   //保留字
    TO, STEP, DRAW, FROM, FOR, //保留字
    T,                          //变量
    SEMICO, L_BRACKET, R_BRACKET, COMMA,// 分隔符
    PLUS, MINUS, MUL, DIV, POWER,		// 运算符
    FUNC,				  // 函数（调用）
    CONST_ID,			  // 常数
    NONTOKEN,			  // 空记号（源程序结束）
    ERRTOKEN			  // 出错记号（非法输入）
};

struct Token{
    enum Token_Type type;
    char *lexeme;
    double value;
    double (*FuncPtr)(double);
};

static struct Token TokenTab[] =
{	{CONST_ID,	"PI",		3.1415926,	NULL},
    {CONST_ID,	"E",		2.71828,	NULL},
    {T,         "T",		0.0,		NULL},
    {FUNC,		"SIN",		0.0,		sin},
    {FUNC,		"COS",		0.0,		cos},
    {FUNC,		"TAN",		0.0,		tan},
    {FUNC,		"LN",		0.0,		log},
    {FUNC,		"EXP",		0.0,		exp},
    {FUNC,		"SQRT",		0.0,		sqrt},
    {ORIGIN,	"ORIGIN",	0.0,		NULL},
    {SCALE,		"SCALE",	0.0,		NULL},
    {ROT,		"ROT",		0.0,		NULL},
    {IS,		"IS",		0.0,		NULL},
    {FOR,		"FOR",		0.0,		NULL},
    {FROM,		"FROM",		0.0,		NULL},
    {TO,		"TO",		0.0,		NULL},
    {STEP,		"STEP",		0.0,		NULL},
    {DRAW,		"DRAW",		0.0,		NULL}
};


#endif /* Token_h */
