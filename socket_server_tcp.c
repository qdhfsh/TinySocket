#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>

#define PORT 2345
#define BACKLOG 5

int main(){
	int iSocketFD = 0;
	int iRecvLen = 0;
	int new_fd = 0;
	char buf[4096] = {0};
	struct sockaddr_in stLocalAddr = {0};
	struct sockaddr_in stRemoteAddr = {0};
	socklen_t socklen = 0;

	iSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(iSocketFD < 0){
		perror("Create socket fail!\n");
		exit(-1);
	}

	stLocalAddr.sin_family = AF_INET;
	stLocalAddr.sin_port = htons(PORT);
	stLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind address struct and socket
	if(bind(iSocketFD, (void *)&stLocalAddr, sizeof(stLocalAddr)) < 0){
		perror("Bind failure!\n");
		exit(-1);
	}

	//start listen, the second argument is maximum listen number
	if(listen(iSocketFD, BACKLOG) < 0){
		perror("Listen failure!\n");
		exit(-1);
	}

	printf("iSocketFD: %d\n", iSocketFD);

	new_fd = accept(iSocketFD, (void *)&stRemoteAddr, &socklen);
	if(new_fd < 0){
		perror("Accept failure!\n");
		exit(-1);
	} else {
		printf("Accpet success!\n");
		send(new_fd, "Hello Socket", sizeof("Hello Socket"), 0);
	}

	while(1){
		iRecvLen = recv(new_fd, buf, sizeof(buf), 0);
		if(iRecvLen > 0){
			printf("buf: %s\n", buf);
			iRecvLen = 0;
		} else {
			printf("Recieve failure or closed connection!\n");
			exit(-1);
		}
	}

	close(new_fd);
	close(iSocketFD);

	return 0;
}
