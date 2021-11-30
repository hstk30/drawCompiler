//
//  main.c
//  CompilerDraw
//
//  Created by apple on 2016/12/12.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "myParser.h"
#define FILE_PATH "/Users/apple/Documents/编程/CompilerDraw/CompilerDraw/drawTest.txt"

int main(int argc, const char * argv[]) {
    // insert code here...
//    struct Token token;
//    
//    if(initScan(FILE_PATH) == 0)
//    {
//        printf("initScan fail\n");
//        return 0;
//    }
//    
//    printf("记号类别    字符串      常数值      函数指针\n");
//    printf("____________________________________________\n");
//    while(1)
//    {
//        token = Scan();		// 通过词法分析器获得一个记号
//        
//        if(token.type != NONTOKEN)	// 打印记号的内容
//            printf("%4d,%12s,%12f,%12x\n",
//                   token.type, token.lexeme, token.value, (unsigned int)token.FuncPtr);
//        else
//            break;			// 源程序结束，退出循环
//    };
//    printf("____________________________________________\n");
//    closeScan();
    
    Parser(FILE_PATH);
    
    return 0;
}
