#include"common.h"
//extern FILE* logfd;

void send_pak(int type, int sock, int x, int y, char player_tpye, char* message)
//发送数据包
{
	//printf("发包0,sock=%d\n",sock);
	MSGPAK* msg_send =(MSGPAK*)malloc(sizeof(MSGPAK));
	msg_send->type=type;
	msg_send->isstart=isstart;
	msg_send->sock=sock;
	msg_send->x=x;
	msg_send->y=y;
	msg_send->player_tpye=player_tpye;
	strcpy(msg_send->message,message);
	//fprintf(logfd,"发包a,sock=%d,type=%d\n",msg_send->sock,msg_send->type);
	write(sock,msg_send,sizeof(MSGPAK));
	//printf("发包b\n");
	free(msg_send);
}

