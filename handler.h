#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

void hello(void);
void GetWeb(int fd);
void GetImage(int fd);
void handler(int fd);
