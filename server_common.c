/*************************************************************************
	> File Name: server_common.c
	> Author: Left
	> Mail: 1059051242@qq.com
	> Created Time: Sat 12 Nov 2022 05:29:00 PM CST
 ************************************************************************/

#include<stdio.h>
#include"common.h"
#include"server_common.h"
//extern int maxsd;
extern FILE* logfd;

int create_listen(int port)  //创建监听
{
	if(CLIENT_NUM < MIN_CLIENT_NUM || CLIENT_NUM > MAX_CLIENT_NUM)
	{
		fprintf(logfd, "CLIENT_NUM is more than %d or less than %d,please check.",
                MAX_CLIENT_NUM, MIN_CLIENT_NUM);
		fprintf(stderr, "CLIENT_NUM is more than %d or less than %d,please check.",
                MAX_CLIENT_NUM, MIN_CLIENT_NUM);
		exit(1);
	}

	if(PLAYER_NUM < MIN_PLAYER_NUM || PLAYER_NUM > MAX_PLAYER_NUM)
	{
		fprintf(logfd, "PLAYER_NUM is more than %d or less than %d,please check.",
                MAX_PLAYER_NUM, MIN_PLAYER_NUM);
		fprintf(stderr, "PLAYER_NUM is more than %d or less than %d,please check.",
                MAX_PLAYER_NUM, MIN_PLAYER_NUM);
		exit(1);
	}
	int sock;
	SA_in sa;
	/*设置通信类型*/
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(logfd,"socket create error!");
		fprintf(stderr,"socket create error!");
		exit(1);
	}
	
	/*设置通信参数*/
	sa.sin_family=AF_INET; //协议，IPV4
	sa.sin_addr.s_addr=htonl(INADDR_ANY); //IP地址，INADDR_ANY表示接受任意IP连接
	sa.sin_port=htons(PORT);    //端口号，在common.h里设置
	
	if((bind(sock,(pSA)&sa,sizeof(sa)))==-1) //将sock和通信参数绑定
	{
		fprintf(logfd,"socket create error!:%s",strerror(errno));
		fprintf(stderr,"socket bind error!");
		exit(1);
	}
	
	if((listen(sock,CLIENT_NUM+1))==-1)  //player_num：玩家数，在common.h里设置
	{
		fprintf(logfd,"socket listen error!:%s",strerror(errno));
		fprintf(stderr,"socket listen error!");
		exit(1);
	}
	return sock;
}

int deal_player_action(int client_sock,int* retx,int* rety,char retmsg[])
//返回类型：0表示正常,-1表示返回错误信息，-2表示非法请求;retmsg表示返回信息
{
	FD_ZERO(&revset);
	FD_ZERO(&t_revset);
	FD_SET(client_sock,&revset);
	//MSGPAK 数据包结构体，在 common.h 中定义
	MSGPAK* msg_recv=(MSGPAK*)malloc(sizeof(MSGPAK));
	while(1)
	{
		t_revset=revset;
		select(client_sock+1,&t_revset,NULL,NULL,NULL);
		if(FD_ISSET(client_sock,&t_revset))
			break;
	}
	if((read(client_sock,msg_recv,sizeof(MSGPAK)))<=0)
	{
		//printf("空数据！\n");
		sprintf(retmsg,"请求非法！请重试！\n");
		return -2;
	}
	*retx=msg_recv->x,*rety=msg_recv->y;//更新返回坐标
	//printf("返回的坐标为：%d %d\n",*retx,*rety);
	if(msg_recv->x<0||msg_recv->x>=N||msg_recv->y<0||msg_recv->y>=N)//检查坐标合法性
	{
		sprintf(retmsg,"坐标非法！请重试！\n");
		
		return -1;
	}
	else if(table[msg_recv->x][msg_recv->y]!='_')//检查该坐标上有无棋子
	{
		sprintf(retmsg,"该坐标上以及有棋子了！请重试！\n");
		return -1;
	}
	else
	{
	//printf("table[%d][%d]=%c\n",msg_recv->x,msg_recv->y,msg_recv->player_tpye);
	}
	return 0;
}

