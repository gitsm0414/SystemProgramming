#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
	printf("before execl..\n");
	
	if(execlp("ls", "ls", "-a", (char *)NULL) == -1){
		perror("execlp"); exit(1);
	}

	printf("if succid, this won't be printed.");

	return 0;
}
