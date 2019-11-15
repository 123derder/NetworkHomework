#include"handler.h"


const int BUFSIZE=16385;


struct{
	char *ext;
	char *filetype;
}extensions[]={
	{"gif","image/gif"},
	{"jpg","image/jpeg"},
	{"jpeg","image/jpeg"},
	{"png","image/png"},
	{"zip","image/zip"},
	{"gz","image/gz"},
	{"tar","image/tar"},
	{"htm","text/html"},
	{"html","text/html"},
	{"exe","text/plain"},
	{0,0}
	};

void hello(void){
	printf("hello~~~~\n");
}



void handler(int fd){

	int j,file_fd,buflen,len;
	long i,ret;
	char *fstr;
	char buffer[BUFSIZE+1],buffer2[BUFSIZE+1]; 	
	memset(buffer,0,sizeof(buffer));
	ret=read(fd,buffer,BUFSIZE); //read web request
	if(ret==0||ret==-1){
		printf("AAA connect error!!!\n");
		exit(3);
	}
	
	if(ret>0&&ret<BUFSIZE)
		buffer[ret]=0;
	else
		buffer[0]=0;
	//printf("QQQQ %s\n!!!!!!!!!!!!!!!!\n",buffer);
	//remove '\r' '\n'
	for(i=0;i<ret;i++)
		if(buffer[i]=='\r'||buffer[i]=='\n')
			buffer[i]=0;
	
	// GET POST
	if(!strncmp(buffer,"GET",3)||!strncmp(buffer,"get",3)){
		printf("\n\nenter GET\n\n");
		for(i=4;i<BUFSIZE;i++)
			if(buffer[i]==' '){
				buffer[i]=0;
				break;
			}
	
		//  block  ../
		for(j=0;j<i-1;j++)
			if(buffer[j]=='.'&&buffer[j+1]=='.')
				exit(3);
		// when web request index.html
		if(!strncmp(&buffer[0],"GET /\0",6)||!strncmp(&buffer[0],"get /\0",6))
			strcpy(buffer,"GET /index.html\0");

		// check what web want
		buflen=strlen(buffer);
		fstr=(char*)0;

		for(i=0;extensions[i].ext!=0;i++){
			len=strlen(extensions[i].ext);
			if(!strncmp(&buffer[buflen-len],extensions[i].ext,len)){
				fstr=extensions[i].filetype;
				break;
			}
		}
		
		//not find the file type
		if(fstr==0)
			fstr=extensions[i-1].filetype;
		
	
		//open file
		if((file_fd=open(&buffer[5],O_RDONLY))==-1)
			write(fd,"Failed to open file\n",20);
		
		// send success (200 and format)
		sprintf(buffer,"HTTP/1.1 200 OK\r\nContent-Type:%s\r\n\r\n",fstr);
		write(fd,buffer,strlen(buffer));
		
		//send content
		while((ret=read(file_fd,buffer,BUFSIZE))>0){
			write(fd,buffer,ret);
		}
		close(file_fd);

	}
	else if(!strncmp(buffer,"POST",4)||!strncmp(buffer,"post",4)){
		char boundary[256]={};
		long data_length=0;
		i=0;
		while(strncmp(&buffer[i],"boundary",8))
			i++;
		i+=13;
		sprintf(boundary,"%s",&buffer[i]);
	//	printf("!!!boundary:%s\n",boundary);
		if(ret>0)
			read(fd,buffer,BUFSIZE);
		
	
		char filename[256]={};
		int file_fd;
		long tempi;
		i=10;
		while(strncmp(&buffer[i],"filename",8)){
			i++;
		}
		tempi=i+10;
		for(i=tempi;i<BUFSIZE;i++){
			if(buffer[i]=='\"'){
				buffer[i]=0;
				break;
			}
		}
		sprintf(filename,"%s",&buffer[tempi]);
	//	printf("!!! post filename:%s\n",filename);
		
		while(strncmp(&buffer[i],"Content-Type",12)){
			i++;
		}
		i+=20;
		while(buffer[i]<'z'&&buffer[i]>'a')
			i++;
		file_fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC);
		
		i+=4;
			
		//	write(file_fd,&buffer[i],BUFSIZE-i-1-strlen(boundary)-10);
		int check=0;
		long tempret=ret;
		long k;
	/*	while((ret=read(fd,buffer2,BUFSIZE))>0){
		printf("ret:%ld\ntempret:%ld\nBUFSIZE:%d\n",ret,tempret,BUFSIZE);
			check=1;
			write(file_fd,&buffer[i],tempret-i);	
			i=0;
			//printf("ret:%ld\nBUFSIZE:%d\n",ret,BUFSIZE);
			//memset(buffer,0,sizeof(buffer));	
			//memcpy(buffer,buffer2,ret);
			for(k=0;k<BUFSIZE+1;k++)
				buffer[k]=buffer2[k];
			memset(buffer2,0,sizeof(buffer2));
			tempret=ret;
				
		}*/
/*		if(!check)
			write(2,&buffer[i],BUFSIZE-i-1-strlen(boundary)-10);
*/		
	//	printf("AAAAAAAAAAAAA\n");
		tempi=i;
		while(strncmp(&buffer[tempi],boundary,strlen(boundary))){
			tempi++;
		}
		tempi-=7;
		printf("tempi:%ld\n",tempi);	
		write(file_fd,&buffer[i],tempi-i+1);
		//write(fd,"HTTP/1.1 200 OK\r\n\r\n",19);
		close(file_fd);
	}
	else{exit(3);}
	exit(0);
}
