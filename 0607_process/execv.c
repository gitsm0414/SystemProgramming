#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
	char *argv[3];

	argv[0] = "ls";
	argv[1] = "-a";
	argv[2] = NULL;
	
	printf("before execl..\n");
	
	if(execv("/bin/ls", argv) == -1){
		perror("execv"); exit(1);
	}

	printf("if succid, this won't be printed.");

	return 0;
}

