//
//  Scanner.h
//  CompilerDraw
//
//  Created by apple on 2016/12/12.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef Scanner_h
#define Scanner_h

#include "Token.h"

int initScan(char *);
struct Token Scan(void);
int closeScan(void);
//void test(void);
void readch(void);
int readNextIs(char c);

#endif /* Scanner_h */
