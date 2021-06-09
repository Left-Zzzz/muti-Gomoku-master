#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#endif

#define N 10   //棋盘大小
#define CLIENT_NUM 4  //房间可容纳客户端数量，最大支持26个玩家游玩，64个客户端在线
#define PLAYER_NUM 3  //房间可容纳玩家数量，最大支持26个玩家游玩
#define IP "127.0.0.1"
#define PORT 1888

typedef struct sockaddr* pSA;
typedef struct sockaddr_in SA_in;
typedef struct Message_pakage //发送消息的结构体
{
	int type;//消息类型：0表示可以下棋，1表示游戏状态更新，2表示提示信息，3表示游戏结束，-1表示错误信息
	int isstart;//判断游戏开始标志
	int sock;//目标sock
	int x,y; //坐标
	char player_tpye;//玩家类型，用于作为棋盘标记
	char message[256];//消息内容
} MSGPAK;


extern int create_listen(int port);
extern int connet_server();
extern int deal_player_action(int client_sock,int* retx,int* rety,char* retmsg);
extern void send_pak(int type,int sock,int x,int y,char player_tpye,char* message);
extern int deal_recv_pak(int sock,int is_server);
extern void refresh_table();
extern int checkWin(int x,int y,char gomoku[][N],char flagWin);

extern int isstart;//判断游戏开始标志
int player_on_sock[26];//最高允许26个玩家一起游戏
int clients[CLIENT_NUM];//允许CLIENT_NUM个客户端在线
char table[N][N];
fd_set liveset, tempset,revset,t_revset,sendset; // 客户端 sockets 描述符集 

