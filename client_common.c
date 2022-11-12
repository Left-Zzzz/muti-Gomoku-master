/*************************************************************************
	> File Name: client_common.c
	> Author: Left
	> Mail: 1059051242@qq.com
	> Created Time: Sat 12 Nov 2022 05:14:19 PM CST
 ************************************************************************/

#include<stdio.h>
#include"common.h"

/*客户端连接到服务器
 *
 */
int connect_server(char * addr,int port)
{
	int sock;
	SA_in sa; // struct sockaddr_in, 在common.h中定义
	
	/*设置通信类型*/
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"socket create error!");
		exit(1);
	}
	
	/*设置通信参数*/
	sa.sin_family=AF_INET; //协议，IPV4
	sa.sin_addr.s_addr=inet_addr(IP); //IP地址
	sa.sin_port=htons(PORT);    //端口号
	if((connect(sock,(pSA)&sa,sizeof(sa)))==-1)
	{
		fprintf(stderr,"fail to connect server!");
		exit(1);
	}
	
	return sock;
}

/*更新棋盘信息
 *
 */
void refresh_table()
{
	system("clear");
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			printf("%c ",table[i][j]);
		}
		printf("\n");
	}
}

