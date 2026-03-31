#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
	pid_t pid;
	int status;

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	if(pid == 0){
		printf("-->child process\n");
		sleep(3);
		exit(3);
	}

	printf("--> parent process\n");
	while(waitpid(pid, &status, WNOHANG) == 0){
		printf("parent still waiting...\n");
		sleep(1);
	}
	printf("child exit status: %d\n", WEXITSTATUS(status));

	return 0;
}
