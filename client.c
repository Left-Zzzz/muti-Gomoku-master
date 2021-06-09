#include "common.h"
int isstart=0;
FILE* logfd;//日志，暂时无效
int main()
{
	logfd=fopen("clientlog.txt","w");
	char msg[256];
	MSGPAK* pak=calloc(1,sizeof(MSGPAK));
	int sock=0,ret,x=-1,y=-1;
	sock=connect_server(IP,PORT);//IP和port:在common.h中设置
	if(sock==-1)
	{
		fprintf(stderr,"连接服务器失败！\n");
		perror("error");
		return -1;
	}
	//printf("连接服务器成功！%d\n",sock);
	printf("连接服务器成功！\n");
	//获取棋盘信息
	if((read(sock,table,sizeof(table)))<=0)
	{
		fprintf(stderr,"初始化棋盘失败！");
		return -1;
	}
	
	//printf("pak->type=%d\n",pak->type);
	while(1)
	{
		FD_ZERO(&revset);
		FD_ZERO(&t_revset);
		FD_SET(sock,&revset);
		
		while(1)
		{
			t_revset=revset;
			select(sock+1,&t_revset,NULL,NULL,NULL);
			if(FD_ISSET(sock,&t_revset))
				break;
		}
		//printf("read pak ing...\n");
		if((read(sock,pak,sizeof(MSGPAK)))<=0)
		{
			perror("读取服务器信息失败!错误信息：");
			exit(-1);
		}
		//读取数据包类型.
		//类型(type)：0表示可以下棋，1表示游戏状态更新，2表示提示信息,3表示游戏结束，-1表示错误信息
		if(!pak)
		{
			fprintf(stderr,"读取服务器信息失败!\n");
		}
		if(pak->type==-1)
		{
			refresh_table();
			//printf("-1\n");
			fprintf(stderr,"%s",pak->message);
			exit(-1);
		}
		else if(pak->type==1)
		{
			//printf("11 %d %d %c \n",pak->x,pak->y,pak->player_tpye);
			if(pak->x>=0&&pak->x<N&&pak->y>=0&&pak->y<N)
			{
				table[pak->x][pak->y]=pak->player_tpye;
				//printf("%c \n",&table[pak->x][pak->y]);
				usleep(500);
			}
			if(pak->isstart==1)
				refresh_table();
			printf("%s",pak->message);
		}
		else if(pak->type==2)
		{
			//printf("2\n");
			printf("%s",pak->message);
			
		}
		else if(pak->type==0)
		{
			//printf("type=%d\n",pak->type);
			printf("%s",pak->message);
			printf("现在是你的回合，可以开始下棋了(格式：x y （注意x和y之间有空格）)：\n");
			scanf("%d %d",&x,&y);
			if(x<0||x>=N||y<0||y>=N)
				x=-1,y=-1;
			send_pak(1,sock,x,y,'A',&msg);
			//printf("sucess\n");
		}
		else if(pak->type==3)
		{
			//printf("3\n");
			table[pak->x][pak->y]=pak->player_tpye;
			printf("%s",pak->message);
			break;
		}
	}
	fclose(logfd);
	return 0;
}

