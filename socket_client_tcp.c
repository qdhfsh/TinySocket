#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>

#define PORT 2345
#define ADDR "47.101.207.44"

int main(){
	int iSocketFD = 0;
	unsigned int iRemoteAddr = 0;
	struct sockaddr_in stRemoteAddr = {0};
	socklen_t socklen = 0;
	char buf[4096] = {0};

	iSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(iSocketFD < 0){
		perror("Create socket faiure!\n");
		exit(-1);
	}

	stRemoteAddr.sin_family = AF_INET;
	stRemoteAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, ADDR, &iRemoteAddr);
	stRemoteAddr.sin_addr.s_addr = iRemoteAddr;

	if(connect(iSocketFD, (void *)&stRemoteAddr, sizeof(stRemoteAddr)) < 0){
		perror("Connect failure!\n");
		exit(-1);
	} else {
		printf("Connect success!\n");
		recv(iSocketFD, buf, sizeof(buf), 0);
		printf("Welcome:%s\n", buf);
	}

	while(1){
		scanf("%s", &buf);
		if(strcmp(buf, "quit") == 0){
			exit(0);
		}
		send(iSocketFD, (void *)&buf, sizeof(buf), 0);
	}

	close(iSocketFD);
	return 0;
}
