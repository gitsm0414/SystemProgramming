#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
	int rfd, wfd, n;
	char buf[10];

	rfd = open("hello.txt", O_RDONLY);
	if(rfd == -1){
		perror("Open hello.txt");
		exit(1);
	}

	wfd = open("hello.bak", O_CREAT | O_WRONLY | O_TRUNC, 0644); // WR_\_R_\_R_
	if(wfd == -1){
		perror("Open hello.bak"); //.bak is back-up
		exit(1);
	}

	while((n = read(rfd, buf, 6)) > 0){
		if(write(wfd, buf, n) != n) 
		perror("Write");
	}
	if(n == -1) perror("Write");

	close(rfd);
	close(wfd);
	return 0;
}
