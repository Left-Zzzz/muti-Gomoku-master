/*************************************************************************
	> File Name: server_common.h
	> Author: Left
	> Mail: 1059051242@qq.com
	> Created Time: Sat 12 Nov 2022 05:08:28 PM CST
 ************************************************************************/

#ifndef _SERVER_COMMON_H
#define _SERVER_COMMON_H

#define CLIENT_NUM     4      // 房间可容纳客户端数量，最大支持26个玩家游玩，64个客户端在线
#define MAX_CLIENT_NUM 64     // 勿动，房间最大可容纳客户端数量
#define MIN_CLIENT_NUM 2      // 勿动，房间最小可容纳客户端数量
#define PLAYER_NUM     3      // 房间可容纳玩家数量，最大支持26个玩家游玩
#define MAX_PLAYER_NUM 26     // 勿动，游戏开启最大可容纳玩家数量
#define MIN_PLAYER_NUM 2      // 勿动，游戏开启最小需要玩家数量
extern int create_listen(int port);

//TODO: 把以下接口移动到新文件
extern int deal_player_action(int client_sock,int* retx,int* rety,char* retmsg);
// 好像没用
//extern int deal_recv_pak(int sock,int is_server);
extern int checkWin(int x,int y,char gomoku[][N],char flagWin);

int clients[CLIENT_NUM];  // 允许CLIENT_NUM个客户端在线
int player_on_sock[26];   // 最高允许26个玩家一起游戏
fd_set liveset, tempset, revset, t_revset; // 客户端 sockets 描述符集 

#endif

