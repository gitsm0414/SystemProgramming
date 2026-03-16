#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
	int fd, fd1;

	fd = open("tmp.aaa", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(fd == -1){
		perror("Create tmp.aaa");
		exit(1);
	}

	close(1); //close stdout
	
	//duplicate fd, fd1 will get 1 ==> redirect standard output to the file
	fd1 = dup(fd);
	
	//printf 속 내용이 모니터가 아니라 tmp.aaa에 출력될 것이다.
	printf("DUP FD=%d\nStandard output redirection\n", fd1);
	close(fd);

	return 0;
}
