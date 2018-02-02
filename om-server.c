#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2020
int main(int argc, char *argv[]){
	fd_set om;
	fd_set read_om;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int fdmax;
	int listener;
	int newfd;
	char buf[1024];
	int nbytes;
	int yes = 1;
	int addrlen;
	int i, j;
	FD_ZERO(&om);
	FD_ZERO(&read_om);
	if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Server-socket() error lol!");
		exit(1);
	}
	printf("Server-socket() is OK...\n");
	if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("Server-setsockopt() error lol!");
		exit(1);
	}
	printf("Server-setsockopt() is OK...\n");

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);
	memset(&(serveraddr.sin_zero), '\0', 8);

    if(bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
		perror("Server-bind() error lol!");
		exit(1);
	}
	printf("Server-bind() is OK...\n");
	if(listen(listener, 10) == -1){
		perror("Server-listen() error lol!");
		exit(1);
	}
	printf("Server-listen() is OK...\n");
	FD_SET(listener, &om);
	fdmax = listener; /* so far, it's this one*/

	for(;;){
		read_om = om;
		if(select(fdmax+1, &read_om, NULL, NULL, NULL) == -1){
		perror("Server-select() error lol!");
		exit(1);
		}
		printf("Server-select() is OK...\n");
		for(i = 0; i <= fdmax; i++){
			if(FD_ISSET(i, &read_om)){
				if(i == listener){
					addrlen = sizeof(clientaddr);
					if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1){
						perror("Server-accept() error lol!..visit- Nxtspace.blogspot.in\n");
					}

					else{
						printf("Server-accept() is OK...Welcome to Nxt Space...\n");
						FD_SET(newfd, &om);
						if(newfd > fdmax){
							fdmax = newfd;
						}
						printf("%s: New connection from %s on socket %d\n", argv[0], inet_ntoa(clientaddr.sin_addr), newfd);
					}
				}
				else{
					nbytes = recv(i, buf, sizeof(buf), 0);
					printf("%s: received from socket %d\n", argv[0], i);
					if((argv[0],i) == (argv[0],4)){
					if(nbytes <= 0){
						if(nbytes == 0)
							printf("%s: socket %d hung up\n", argv[0], i);
						else
							perror("recv() error lol!");
					close(i);
					FD_CLR(i, &om);
				}
			else{
				for(j = 0; j <= fdmax; j++){
					if(FD_ISSET(j, &om)){
						if(j != listener && j != i){
							if(send(j, buf, nbytes, 0) == -1)
								perror("send() error lol!");
						}
					}
				}
			}
					}
					else printf("%s: User trying to hack connected on socket %d\n", argv[0], i);
				}
			}
		}
	}
return 0;
}
