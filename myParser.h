//
//  myParser.h
//  CompilerDraw
//
//  Created by apple on 2016/12/12.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef myParser_h
#define myParser_h
#include "Scanner.h"

#include <stdlib.h>
#include <stdarg.h>

typedef double (* FuncPtr)(double);

struct ExprNode
{  enum Token_Type OpCode;	// 记号种类
    union
    {
        struct { struct ExprNode *Left, *Right;
        } CaseOperator;	// 二元运算
        struct { struct ExprNode * Child;
            FuncPtr MathFuncPtr;
        } CaseFunc;	// 函数调用
        double CaseConst; 	// 常数，绑定右值
        double * CaseParmPtr; 	// 参数T，绑定左值
    } Content;
};


void Parser(char * SrcFilePtr);
void Program();
void Statement();
void OriginStatement();
void RotStatement();
void ScaleStatement();
void ForStatement();

struct ExprNode * Expression();
struct ExprNode * Term();
struct ExprNode * Factor();
struct ExprNode * Component();
struct ExprNode * Atom();
#endif /* myParser_h */
