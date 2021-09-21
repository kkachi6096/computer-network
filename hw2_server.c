#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int str_len, i;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[BUF_SIZE]="";
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
		error_handling("bind() error"); 
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr); 
       	printf("---------------------------\n");
	printf(" Address Conversion Server\n");
	printf("---------------------------\n");

	for(i=0; i<5; i++){	
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
		if(clnt_sock==-1)
			error_handling("accept() error"); 


		while((str_len=read(clnt_sock, message, BUF_SIZE))!=0){
			if(!strncmp(message,"quit",str_len)){
				printf("quit received and exit program!\n");
				write(clnt_sock, message, str_len);
				close(clnt_sock);
				close(serv_sock);
				exit(1);
			}
			printf("Received Dotted-Decimal Address: %s\n", message);
			
			struct sockaddr_in addr_inet;

			unsigned long conv_addr=inet_addr(message);
			
			if(conv_addr==INADDR_NONE)
				printf("Address conversion fail: Format error.\n\n");
			
			else{
				char *str_ptr;
				char str_arr[BUF_SIZE];

				if(!inet_aton(message, &addr_inet.sin_addr))
					error_handling("Conversion error");
				else
					printf("inet_aton: %s -> %#x \n", message, addr_inet.sin_addr.s_addr);
				str_ptr=inet_ntoa(addr_inet.sin_addr);
				strcpy(str_arr,str_ptr);
				printf("inet_ntoa: %#x -> %s \n", addr_inet.sin_addr.s_addr, str_arr);
				printf("Address conversion success.\n\n");
			}
			write(clnt_sock, message, str_len);
		}

		close(clnt_sock);

	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
