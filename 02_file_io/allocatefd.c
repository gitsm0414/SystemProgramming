#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

int openFile(void){
	int fd = open("hello.txt", O_RDWR);
	if(fd == -1){
		perror("File open");
		exit(1);
	}
	return fd;
}

int main(void){
	int fd = 0;

	fd = openFile();
	printf("fd= %d\n", fd);
	close(fd);

	close(0);

	fd = openFile();
	printf("fd= %d\n", fd);
	close(fd);

	return 0;
}
