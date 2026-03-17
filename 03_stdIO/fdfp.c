#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>


int main(void){
	FILE* fp;
	int fd;
	char buf[BUFSIZ];

	fd = open("hello.txt", O_RDONLY);
	if(fd == -1){
		perror("open");
		exit(1);
	}

	fp = fdopen(fd, "r");

	fgets(buf, BUFSIZ, fp);
	printf("Read: %s\n", buf);

	fclose(fp);

	return 0;
}
