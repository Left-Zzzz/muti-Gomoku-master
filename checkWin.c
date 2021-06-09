/*************************************************************************
> File Name: checkWin.c
> Author: CarlSun
> Mail:sunxiuyang04@gmail.com 
> Created Time: Mon Aug 22 21:35:49 2016
 ************************************************************************/


#include"gomoku.h"

int checkWin(int x,int y,char gomoku[][N],char flagWin){
	int i,j,cou1=0,cou2=0;
	for(i=y+1;i<N;i++)//->
		if(gomoku[x][i]==flagWin)
			cou1++;
		else break;
	for(i=y-1;i>=0;i--)
		if(gomoku[x][i]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;
	cou2=cou1>cou2?cou1:cou2;
	cou1=0;



	for(i=x+1;i<N;i++)
		if(gomoku[i][y]==flagWin)
			cou1++;
		else break;
	for(i=x-1;i>=0;i--)
		if(gomoku[i][y]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;
	cou2=cou1>cou2?cou1:cou2;
	cou1=0;



	for(i=x+1,j=y+1;i<N,y<N;i++,j++)
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(i=x-1,j=y-1;i>=0,j>=0;i--,j--)
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;
	cou2=cou1>cou2?cou1:cou2;
	cou1=0;



	for(i=x-1,j=y+1;i>=0,j<N;i--,j++)
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(i=x+1,j=y-1;i<N,j>=0;i++,j--)
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			printf("%c ",gomoku[i][j]);
		}
		printf("\n");
	}
	cou2=cou1>cou2?cou1:cou2;
	//printf("%c 的最长连续棋子数为：%d\n",flagWin,cou2);
	if(cou1>=4) return 1;
	return 0;
}

