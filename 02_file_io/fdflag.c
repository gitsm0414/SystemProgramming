#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>


int main(void){
	int fd, flags;

	fd = open("linux.txt", O_RDWR); //파일은 현재 시작점부터 읽게된다.
	if(fd == -1){
		perror("Opening linux.txt");
		exit(1);
	}

	if((flags = fcntl(fd, F_GETFL)) == -1){
		perror("fcntl getflag");
		exit(1);
	}
	
	flags |= O_APPEND; // add append mode
	
	if((fcntl(fd, F_SETFL, flags)) == -1){
		perror("fcntl setflag");
		exit(1);
	}

	if((write(fd, "KOREATECH", 9)) != 9){
		perror("write");
		exit(1);
	}

	close(fd);

	return 0;
}

