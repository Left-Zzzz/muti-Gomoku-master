/*************************************************************************
> File Name: gomoku.h
> Author: CarlSun
> Mail:sunxiuyang04@gmail.com 
> Created Time: Sat Aug 20 20:28:12 2016
 ************************************************************************/

#ifndef _5ZIQI_
#define _5ZIQI_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"common.h"
int checkWin(int x,int y,char gomoku[][N],char flagWin);
typedef struct tag{
		int x,y,xl,yl;
}node;
#endif
