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

struct MessageData
{
	int id;
	char message[4096];
};

int iSocketFD = 0;
int iRecvLen = 0;
int new_fd = 0;
char buf[4096] = {0};
struct sockaddr_in stLocalAddr = {0};
struct sockaddr_in stRemoteAddr = {0};
socklen_t socklen = 0;

void socket_server_init(){

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

	return ;
}

void socket_server_listen(){
	
	printf("Waiting connect...\n");
	new_fd = accept(iSocketFD, (void *)&stRemoteAddr, &socklen);
	if(new_fd < 0){
		perror("Accept failure!\n");
		exit(-1);
	} else {
		struct MessageData message_welcome;
		message_welcome.id = 123;
		memcpy(message_welcome.message, "This is struct welcome message!", sizeof("This is struct welcome message!"));
		printf("Accpet success!\n");
		//send(new_fd, "Hello Socket", sizeof("Hello Socket"), 0);
		send(new_fd, (char *)&message_welcome, sizeof(message_welcome), 0);
	}

	return ;
}

int main(){

	socket_server_init();

	socket_server_listen();

	while(1){
		iRecvLen = recv(new_fd, buf, sizeof(buf), 0);
		if(iRecvLen > 0){
			printf("buf: %s", buf);
			iRecvLen = 0;
		} else {
			printf("User disconnect.\n");
			socket_server_listen();
		}
	}

	close(new_fd);
	close(iSocketFD);

	return 0;
}

