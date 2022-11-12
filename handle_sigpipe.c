/*************************************************************************
	> File Name: sigpipe_handle.c
	> Author: Left
	> Mail: 1059051242@qq.com
	> Created Time: Sat 12 Nov 2022 04:44:30 PM CST
 ************************************************************************/

#include <signal.h>
#include <errno.h>
#include <stdio.h>

void sigpipe_handler();

void handle_sigpipe()
{
    // 缺省处理，忽略该信号
    struct sigaction action;
    action.sa_handler = sigpipe_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGPIPE, &action, NULL);
}

void sigpipe_handler()
{
    perror("发生了一个sigpipe, 错误信息：");
}

