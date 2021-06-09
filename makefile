all: client server

client: client.c linker.c checkWin.c
	gcc -g -w client.c checkWin.c linker.c  -o client

server: server.c linker.c checkWin.c
	gcc -g server.c  linker.c checkWin.c -lpthread -o server
.PHONY:clean
clean:
	rm -f client server
