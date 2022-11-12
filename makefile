all: client server

client: client.c linker.c client_common.c
	gcc -g -w client.c checkWin.c linker.c client_common.c  -o client -g

server: server.c linker.c checkWin.c handle_sigpipe.c
	gcc -g server.c  linker.c checkWin.c server_common.c handle_sigpipe.c -lpthread -o server -g

.PHONY:clean
clean:
	rm -f client server
