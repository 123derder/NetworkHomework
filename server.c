#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include"handler.h"

int main(int argc,char *argv[]){
	
	char inputBuffer[256]={};
	int sockfd=0,forClientSockfd=0;
	int reuseaddr=1;
	int pid=1; 
/*	
	if(fork!=0)
		return 0;
*/
	signal(SIGCLD,SIG_IGN);

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1){
		printf("Fail to creat asocket.\n");
	}
	
	
	
	// SET SOCKET
	struct sockaddr_in ServerInfo,ClientInfo;
	int addrlen=sizeof(ClientInfo);
	bzero(&ServerInfo,sizeof(ServerInfo));

	ServerInfo.sin_family=PF_INET;
	ServerInfo.sin_addr.s_addr=INADDR_ANY;
	ServerInfo.sin_port=htons(4450);
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr));
	if(bind(sockfd,(struct sockaddr*)&ServerInfo,sizeof(ServerInfo))<0){printf("bind error!!!\n");}
	listen(sockfd,64);
	printf("server begin!!!\n");
	hello();
	while(1){
		forClientSockfd=accept(sockfd,(struct sockaddr*)&ClientInfo,&addrlen);
		printf("connnect!!!\n");
		
		 if((pid=fork())==0){
			close(sockfd);
			handler(forClientSockfd);
			close(forClientSockfd);
			exit(3);	
		}else{
			close(forClientSockfd);
		}
	//	handler(forClientSockfd);
	}

return 0;
}
