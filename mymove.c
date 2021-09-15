#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void error_handling(char* message);

int main(int argc, char* argv[]){
	int fd1, fd2, byte;
	char buff[1000];

	if(argc!=3)
		error_handling("[Error] mymove Usage: ./mymove src_file dest_file");
	
	fd1 = open(argv[1], O_RDONLY);
	
	if(fd1 == -1)
		error_handling("open(1) error!");

	fd2 = open(argv[2], O_CREAT|O_RDWR|O_TRUNC, 0644);

	if(fd2 == -1)
		error_handling("open(2) error!");
	
	byte = read(fd1, buff, sizeof(buff));

	if(byte==-1)
		error_handling("read(1) error!");

	if(write(fd2, buff, byte)==-1)
		error_handling("write(2) error!");
	
	remove(argv[1]);
	close(fd1);
	close(fd2);

	printf("move from %s to %s (bytes: %d) finished.\n", argv[1], argv[2], byte);
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
