#include "common.h"
#include <semaphore.h>

MSGPAK pak;

sem_t sem;
struct timeval timeout = {3, 0};
int actual_player,client_num;
int maxsd;//描述符最大值
int isstart,ret_stat,cur_move_sock,target_sock;
int x,y;
void* start_server(void *arg);
void* listen_net(void *arg);
void init_table();
FILE* logfd;
int main()
{
	logfd=fopen("log.txt","w");
	sem_init(&sem,0,1);
	pthread_t thread_listen_net,thread_start_server;
	cur_move_sock=-1,target_sock=0;
	actual_player=0,client_num=0;
	int server_sock,th_ret;
	memset(player_on_sock,0,sizeof(player_on_sock));
	memset(clients,0,sizeof(clients));
	init_table();
	server_sock=create_listen(1888);
	maxsd=server_sock;//描述符最大值
	printf("server start, server_sock=%d\n",server_sock);
	//开始循环监听(创建监听线程)
	th_ret=pthread_create(&thread_listen_net, NULL, listen_net, &server_sock); 
	if(th_ret!=0)
	{
		fprintf(logfd,"create thread error!\n");
		fprintf(stderr,"create thread error!\n");
		return -1;
	}
	//创建游戏主体线程
	th_ret=pthread_create(&thread_start_server, NULL, start_server, &server_sock); 
	if(th_ret!=0)
	{
		fprintf(logfd,"create thread error!\n");
		fprintf(stderr,"create thread error!\n");
		return -1;
	}
	pthread_join(thread_listen_net,NULL);
	pthread_join(thread_start_server,NULL);
	fclose(logfd);
	sem_destroy(&sem);
	return 0;
}

void init_table()
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			table[i][j]='_';
		}
	}
}

void* listen_net(void *arg)
{
	int server_sock=*(int*)arg;
	FD_ZERO(&liveset);
	FD_ZERO(&tempset);
	FD_SET(server_sock,&liveset);
	struct sockaddr_in remoteaddr; /* 客户机地址结构 */
	socklen_t addrlen;
	while(1)
	{	
		sem_wait(&sem);
		//printf("linstiing...\n");
		tempset=liveset;
		if((select(maxsd+1,&tempset,NULL,NULL,&timeout))==0)
		{
			sem_post(&sem);
			usleep(50);
			continue;
		}
		for(int i=0;i<=CLIENT_NUM;i++)
		{
			if(clients[i]<=0||i==cur_move_sock)
				continue;
			if(FD_ISSET(clients[i],&tempset))
			{
				char retmsg[256];
				//返回类型：1表示游戏结束,0表示正常,-1表示错误;retmsg表示返回信息
				ret_stat=deal_player_action(clients[i],&x,&y,retmsg);
				//printf("ret_stat=%d\n",ret_stat);
				if(ret_stat<=0) //客户端断开
				{
					//printf("客户端断开");
					FD_CLR(clients[i], &liveset);
					for(int j=0;j<PLAYER_NUM;j++)//从player_on_sock中删除sock信息
					{
						if(player_on_sock[j]==clients[i])
						{
							player_on_sock[j]=-1;
							printf("玩家 %c 退出游戏。\n",(char)j+'A');
							actual_player--;
						}
						
					}
					
					client_num--;
					//printf("aclient_num=%d,player_num=%d\n",client_num,actual_player);
					clients[i]=-1;   //从clients中删除sock信息
					continue;
				}
					
			}
			
			
		}
		
		if(FD_ISSET(server_sock,&tempset))//如果是一个新的连接
		{
			int  csd,player_cur; // 已连接的 socket 描述符 
			csd = accept(server_sock, (struct sockaddr *) &remoteaddr, &addrlen);
			char message[256];
			//printf("client_num=%d actual_player=%d csd=%d\n",CLIENT_NUM,actual_player,csd);
			if(csd==-1)
			{
				fprintf(logfd,"csd=-1,accept:%s\n",strerror(errno));
				perror("csd=-1,accept");
				exit(0);
			}
			
			//发送棋盘信息
			write(csd,table,sizeof(table));
			//printf("发送棋盘信息\n");
			//消息类型type：0表示可以下棋，1表示游戏状态更新，2表示提示信息，-1表示错误信息
			if (client_num>=CLIENT_NUM)
			{
				printf("服务器人数已满,已拒绝新的客户端加入。\n");
				sprintf(message,"服务器人数已满！\n");
				send_pak(3,csd,-1,-1,-1,message);
				//发送拒绝消息
				continue;
				
			}
			else if(actual_player<PLAYER_NUM&&isstart==0)
			{
				for(int j=0;j<=PLAYER_NUM;j++)//从player_on_sock中加入sock信息
				{
					if(j==PLAYER_NUM)
					{
						fprintf(logfd,"分配玩家空间出错\n");
						fprintf(stderr,"分配玩家空间出错\n");
					}
					//printf("player_cur=%d",j);
					if(player_on_sock[j]<=0)
					{
						player_cur=j;
						player_on_sock[j]=csd;
						//printf("player_on_sock[%d]=%d\n",j,player_on_sock[j]);
						break;
					}
				}
				
				for(int j=0;j<=CLIENT_NUM;j++)//从clients中加入sock信息
				{
					if(j==CLIENT_NUM)
					{
						fprintf(logfd,"分配玩家空间出错\n");
						fprintf(stderr,"分配玩家空间出错b\n");
					}
					if(clients[j]<=0)
					{
						clients[j]=csd;
						//printf("clients[%d]=%d\n",j,clients[j]);
						break;
					}
				}
				//发送进入成功消息
				printf("玩家 %c 加入游戏。\n",(char)player_cur+'A');
				sprintf(message,"加入游戏成功！你是玩家 %c 。请等待管理员开始游戏！\n",(char)player_cur+'A');
				send_pak(1,csd,-1,-1,(char)player_cur+'A',message);
				//更新服务器在线玩家和在线服务器状态
				actual_player++;
				client_num++;
				
			}
			else 
			{
				for(int j=0;j<=CLIENT_NUM;j++)//从clients中加入sock信息
				{
					if(j==CLIENT_NUM)
					{
						fprintf(logfd,"分配玩家空间出错\n");
						fprintf(stderr,"分配玩家空间出错c\n");
					}
					if(clients[j]<=0)
					{
						clients[j]=csd;
						//printf("client[%d]=%d\n",j,clients[j]);
						break;
					}
				}
				
				//发送进入观看区消息
				
				if(isstart==0)
				{
					printf("超过最大玩家数，已把新进入的客户端拉进观战区！\n");
					sprintf(message,"超过最大玩家数，已经把你拉进观战区！\n");
				}
				else
				{
					printf("游戏已开始，已把新进入的客户端拉进观战区！\n");
					sprintf(message,"游戏已开始，已经把你拉进观战区！\n");
				}
				
				send_pak(1,csd,-1,-1,(char)0,message);
				
				//更新服务器在线客户端状态
				client_num++;
			}
			
			
			// 将该连接的套接字描述符 csd 加入liveset 
			FD_SET(csd, &liveset);
			
			//printf("bclient_num=%d,player_num=%d\n",client_num,actual_player);
			// 保持 maxsd 记录的是最大的套接字描述符 
			if (csd > maxsd)
				maxsd = csd;
		}
		sem_post(&sem);
		usleep(50);
	}
	
}
void* start_server(void* arg)
{
	while(1)
	{
		int flag=0,server_sock=*(int*)arg;
		char msg[256]="";
		cur_move_sock=-1;
		while(actual_player<=1);
		printf("请任意键开始游戏。\n");
		scanf("%*s");
		isstart=1;
		sprintf(msg,"游戏开始！\n");
		for(int i=0;i<CLIENT_NUM;i++)  //逐个发送游戏开始消息
		{
			//type(消息类型)：0表示可以下棋，1表示游戏状态更新，2表示提示信息，-1表示错误信息
			if(player_on_sock[i]<=0)
				continue;
			
			send_pak(1,player_on_sock[i],-1,-1,cur_move_sock+'A',msg);
		}
		sleep(1);
		int t=0;//当前待遍历的可执行操作的玩家下标
		while(1)
		{
			while(player_on_sock[t]<=0)
			{
				t=(t+1)%PLAYER_NUM;
			}
			cur_move_sock=t;
			
			sem_wait(&sem);//等待客户端旧的在线情况被刷新
			sem_post(&sem);
			usleep(500);
			sem_wait(&sem);//等待客户端在线情况更新,避免write了一个关闭的fd导致程序退出
			sem_post(&sem);
			
			if(actual_player<=1)
			{
				sprintf(msg,"玩家数量不足，游戏结束！\n");
				init_table();
				isstart=0;
				for(int i=0;i<CLIENT_NUM;i++)
				{
					if(clients[i]>0)
						send_pak(3,clients[i],x,y,(char)cur_move_sock+'A',msg);
				}
				break;
			}
			//printf("test1 player_on_sock[cur_move_sock]=%d\n",player_on_sock[cur_move_sock]);
			char retmsg[256]="";
			//向所有玩家发送信息
			//type(消息类型)：0表示可以下棋，1表示游戏状态更新，2表示提示信息，-1表示错误信息
			sprintf(msg,"现在是玩家 %c 的回合。\n",cur_move_sock+'A');
			for(int i=0;i<CLIENT_NUM;i++)
			{
				if(clients[i]<=0)
				{
						continue;
				}
				if(player_on_sock[cur_move_sock]==clients[i])
				{
					//printf("%d 可以下棋",player_on_sock[i]);
					send_pak(0,clients[i],-1,-1,cur_move_sock+'A',msg);
				}
				else
				{
					send_pak(1,clients[i],-1,-1,cur_move_sock+'A',msg);
				}
			}
			
			ret_stat=deal_player_action(player_on_sock[cur_move_sock],&x,&y,retmsg);
			//printf("test2 ret_stat=%d ,%d %d\n",ret_stat,x,y);
			//ret_stat=0（正常时）的待发送消息
			if(ret_stat==0)
			{
				table[x][y]=(char)cur_move_sock+'A';
				printf("玩家 %c 把他的棋子落在 ( %d , %d ) 处。\n",(char)cur_move_sock+'A',x,y);
				sprintf(msg,"玩家 %c 把他的棋子落在 ( %d , %d ) 处。\n",(char)cur_move_sock+'A',x,y);
			}
			
			//若错误，直接请求命令的客户端返回-1
			//错误值：-1表示返回错误信息，-2表示非法请求.
			if(ret_stat<0)
			{
				printf("%s",retmsg);
				send_pak(1,player_on_sock[cur_move_sock],-1,-1,(char)cur_move_sock+'A',retmsg);
				if(ret_stat==-1)
					continue;
			}
			else//若无错误，则向所有玩家更新消息
			{
				if(ret_stat==0)
				{
					for(int i=0;i<CLIENT_NUM;i++)
					{
						if(clients[i]>0)
							send_pak(1,clients[i],x,y,(char)cur_move_sock+'A',msg);
					}
					sleep(2);
				}
				if(checkWin(x,y,table,(char)cur_move_sock+'A'))//检查游戏是否结束
				{
					for(int i=0;i<CLIENT_NUM;i++)
					{
						if(clients[i]>0)
							send_pak(1,clients[i],x,y,(char)cur_move_sock+'A',msg);
					}
					sprintf(msg,"游戏结束！玩家 %c 获胜！\n",(char)cur_move_sock+'A');
					for(int i=0;i<CLIENT_NUM;i++)
					{
						if(clients[i]>0)
							send_pak(3,clients[i],x,y,(char)cur_move_sock+'A',msg);
					}
					printf("%s",msg);
					break;
				}
			}
			t=(t+1)%PLAYER_NUM;//当前回合可进行操作的玩家下标更新
		}
	}
}
