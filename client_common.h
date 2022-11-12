/*************************************************************************
	> File Name: client_common.h
	> Author: Left
	> Mail: 1059051242@qq.com
	> Created Time: Sat 12 Nov 2022 05:13:50 PM CST
 ************************************************************************/

#ifndef _CLIENT_COMMON_H
#define _CLIENT_COMMON_H

extern int connect_server();
extern void refresh_table();
fd_set revset,t_revset; // 客户端 sockets 描述符集 

#endif
