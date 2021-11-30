//
//  myParser.c
//  CompilerDraw
//
//  Created by apple on 2016/12/12.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "myParser.h"
#include <math.h>
extern int line;  //来自Scanner.c
static double origin_x = 0.0, origin_y = 0.0;
static double scale_x = 1.0, scale_y = 1.0;
static double rot_ang = 0.0;
static double Parameter = 0.0;
static struct Token token;       

static void FetchToken ();
static void MatchToken (enum Token_Type AToken);
static void SyntaxError (int case_of);
static struct ExprNode * MakeExprNode(enum Token_Type opcode, ...);
static void PrintSyntaxTree(struct ExprNode *root, int indent);
static double GetExprVal(struct ExprNode *root);

static void FetchToken()
{
    token = Scan();
    if(token.type == ERRTOKEN)
        SyntaxError(1);
}

static void MatchToken(enum Token_Type AToken)
{
    if(token.type != AToken)
        SyntaxError(0);
    token = Scan();
}

static void SyntaxError(int case_of)
{
    switch (case_of) {
        case 0:
            printf("Error on line%d:, this token '%s' is not reasonable in here \n",
                   line,token.lexeme);
            break;
        case 1:
            printf("Error on line%d:, '%s' is a error token\n", line, token.lexeme);
            break;
        default:
            break;
    }
}

static struct ExprNode * MakeExprNode(enum Token_Type opcode, ...)
{
    struct ExprNode * ExprPtr = malloc(sizeof(struct ExprNode));
    ExprPtr->OpCode = opcode;
    va_list ArgPtr;
    va_start(ArgPtr, opcode);
    switch(opcode)
    {
        case CONST_ID:	// 常数节点
            ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr,double);
            break;
        case T:		// 参数节点
            ExprPtr->Content.CaseParmPtr = &Parameter;
            break;
        case FUNC:	// 函数调用节点
            ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr, FuncPtr);
            ExprPtr->Content.CaseFunc.Child = (struct ExprNode *)va_arg(ArgPtr,struct ExprNode *);
            break;
        default:	// 二元运算节点
            ExprPtr->Content.CaseOperator.Left = (struct ExprNode *)va_arg(ArgPtr,struct ExprNode *);
            ExprPtr->Content.CaseOperator.Right = (struct ExprNode *)va_arg(ArgPtr,struct ExprNode *);
            break;
    }	// end of switch
    va_end(ArgPtr);
    return ExprPtr;
}

static void PrintSyntaxTree(struct ExprNode *root, int indent)
{
    if(root == NULL || root->OpCode == ERRTOKEN)
    {
        return ;
    }
    for(int n = 0; n < indent; n++)
        printf("\t");
    
    switch (root->OpCode) {
        case CONST_ID:
            printf("%f\n", root->Content.CaseConst);
            break;
        case FUNC:
            printf("%x\n", (int)root->Content.CaseFunc.MathFuncPtr);
            PrintSyntaxTree(root->Content.CaseFunc.Child, indent + 1);
            break;
        case T:
            printf("T\n");
            break;
        default:
            printf("%d\n", root->OpCode);
            PrintSyntaxTree(root->Content.CaseOperator.Left, indent + 1);
            PrintSyntaxTree(root->Content.CaseOperator.Right, indent + 1);
            break;
    }
    
}

static double GetExprVal(struct ExprNode *root)
{
    if(root == NULL || root->OpCode == ERRTOKEN)
        return 0.0;
    switch (root->OpCode) {
        case PLUS:
            return GetExprVal(root->Content.CaseOperator.Left) +
            GetExprVal(root->Content.CaseOperator.Right);
        case MINUS:
            return GetExprVal(root->Content.CaseOperator.Left) -
            GetExprVal(root->Content.CaseOperator.Right);
        case MUL:
            return GetExprVal(root->Content.CaseOperator.Left) *
            GetExprVal(root->Content.CaseOperator.Right);
        case DIV:
            return GetExprVal(root->Content.CaseOperator.Left) /
            GetExprVal(root->Content.CaseOperator.Right);
        case POWER:
            return pow(GetExprVal(root->Content.CaseOperator.Left), GetExprVal(root->Content.CaseOperator.Right));
        case FUNC:
            return (* root->Content.CaseFunc.MathFuncPtr)(GetExprVal(root->Content.CaseFunc.Child));
        case CONST_ID:
            return root->Content.CaseConst;
        case T:
            return *(root->Content.CaseParmPtr);
        default:
            return 0.0;
            break;
    }
}

void Parser(char *SrcFilePtr)
{
    if(initScan(SrcFilePtr) == 0)
    {
        printf("open fail!\n");
        return ;
    }
    
    FetchToken();
    Program();
    closeScan();
}

void Program()
{
    printf("enter program\n");
    while(token.type != NONTOKEN)
    {
        Statement();
        MatchToken(SEMICO);
    }
    printf("leave program\n");
}

void Statement()
{
    printf("enter Statement\n");
    switch (token.type) {
        case FOR:
            ForStatement();
            break;
        case ORIGIN:
            OriginStatement();
            break;
        case ROT:
            RotStatement();
            break;
        case SCALE:
            ScaleStatement();
            break;
        default:
            SyntaxError(0);
            break;
    }
    printf("leave Statement\n");
}

//OriginStatment → ORIGIN IS
//L_BRACKET Expression COMMA Expression R_BRACKET
//ScaleStatment  → SCALE IS
//L_BRACKET Expression COMMA Expression R_BRACKET
//RotStatment → ROT IS Expression


void ForStatement ()
{
    printf("enter ForStatement\n");
    double start_x, end_x, hStep;
    struct ExprNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;
    MatchToken (FOR);		MatchToken(T);
    MatchToken (FROM);	start_ptr = Expression();
    
    PrintSyntaxTree(start_ptr, 1);  start_x = GetExprVal(start_ptr);
    printf("the start point is %f\n", start_x);
    MatchToken (TO);    end_ptr = Expression();
    
    PrintSyntaxTree(end_ptr, 1);    end_x = GetExprVal(end_ptr);
    printf("the end point is %f\n", end_x);
    MatchToken (STEP);	step_ptr  = Expression();
    
    PrintSyntaxTree(step_ptr, 1);   hStep = GetExprVal(step_ptr);
    printf("the step is %f\n", hStep);
    MatchToken (DRAW);
    MatchToken (L_BRACKET);	x_ptr = Expression();
    
    PrintSyntaxTree(x_ptr, 1);
    
    MatchToken (COMMA);	y_ptr = Expression();
    
    PrintSyntaxTree(y_ptr, 1);
    
    MatchToken (R_BRACKET);
    printf("leave ForStatement\n");
}

void OriginStatement()
{
    printf("enter OriginStatement\n");
    struct ExprNode *x_ptr, *y_ptr;
    MatchToken(ORIGIN);
    MatchToken(IS);
    MatchToken(L_BRACKET);  x_ptr = Expression();
    
    PrintSyntaxTree(x_ptr, 1);  origin_x = GetExprVal(x_ptr);
    
    MatchToken(COMMA);      y_ptr = Expression();
    
    PrintSyntaxTree(y_ptr, 1);  origin_y = GetExprVal(y_ptr);
    
    MatchToken(R_BRACKET);
    printf("now , the origin is (%f, %f)\n", origin_x, origin_y);
    printf("leave OriginStatement\n");
}

void ScaleStatement()
{
    printf("enter ScaleStatement\n");
    struct ExprNode *x_scale, *y_scale;
    MatchToken(SCALE);
    MatchToken(IS);
    MatchToken(L_BRACKET);  x_scale = Expression();
    
    PrintSyntaxTree(x_scale, 1);    scale_x = GetExprVal(x_scale);
    
    MatchToken(COMMA);      y_scale = Expression();
    
    PrintSyntaxTree(y_scale, 1);    scale_y = GetExprVal(y_scale);
    
    MatchToken(R_BRACKET);
    printf("now, the scale is (%f, %f)\n", scale_x, scale_y);
    printf("leave ScaleStatement\n");
}

void RotStatement()
{
    printf("enter RotStatement\n");
    struct ExprNode *angle;
    MatchToken(ROT);
    MatchToken(IS);     angle = Expression();
    
    PrintSyntaxTree(angle, 1);  rot_ang = GetExprVal(angle);
    
    printf("now, the rot angle is %f\n", rot_ang);
    printf("leave RotStatement\n");
}

//Expression 	→ Term  { ( PLUS | MINUS) Term }
//Term       	→ Factor { ( MUL | DIV ) Factor }
//Factor  	→ PLUS Factor | MINUS Factor | Component
//Component 	→ Atom [POWER Component]
//Atom → CONST_ID
//| T
//| FUNC L_BRACKET Expression R_BRACKET
//| L_BRACKET Expression R_BRACKET

struct ExprNode * Expression()
{
    struct ExprNode *left, *right;
    enum Token_Type token_tmp;
    left = Term();
    while (token.type==PLUS || token.type==MINUS)
    {
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Term();
        left = MakeExprNode(token_tmp,left,right);
    }
    return left;
}

struct ExprNode * Term()
{
    struct ExprNode *left, *right;
    enum Token_Type token_tmp;
    left = Factor();
    while(token.type == MUL || token.type == DIV)
    {
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Factor();
        left = MakeExprNode(token_tmp, left, right);
    }
    return left;
}

struct ExprNode * Factor()
{
    struct ExprNode * left, *right;
    enum Token_Type token_tmp;
    if(token.type == PLUS || token.type == MINUS)
    {
        left = malloc(sizeof(struct ExprNode));      //对于一元操作，假设他的二元操作，做操作为0
        left->OpCode = CONST_ID;
        left->Content.CaseConst = 0.0;
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Factor();
        left = MakeExprNode(token_tmp, left, right);
        
    }else
        left = Component();
    
    return left;
}

struct ExprNode * Component()
{
    struct ExprNode *left, *right;
    enum Token_Type token_tmp;
    left = Atom();
    if(token.type == POWER)
    {
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Component();
        left = MakeExprNode(token_tmp, left, right);
    }
    
    return left;
}

struct ExprNode * Atom()
{
    struct ExprNode * Node = malloc(sizeof(struct ExprNode));
    switch (token.type) {
        case CONST_ID:
            Node->OpCode = token.type;
            Node->Content.CaseConst = token.value;
            MatchToken(CONST_ID);
            break;
        case T:
            Node->OpCode = token.type;
            Node->Content.CaseParmPtr = &Parameter;
            MatchToken(T);
            break;
        case FUNC:
            Node->OpCode = token.type;
            Node->Content.CaseFunc.MathFuncPtr = token.FuncPtr;
            MatchToken(FUNC);
            MatchToken(L_BRACKET);
            Node->Content.CaseFunc.Child = Expression();
            MatchToken(R_BRACKET);
            break;
        case L_BRACKET:
            MatchToken(L_BRACKET);
            Node = Expression();
            MatchToken(R_BRACKET);
            break;
        default:
            Node = NULL;
            SyntaxError(0);
            MatchToken(token.type);
            break;
    }
    
    return Node;
}
