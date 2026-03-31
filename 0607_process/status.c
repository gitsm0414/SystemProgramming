#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
	int status;
	pid_t pid;

	switch(pid = fork()){
	case -1://fork failed
		perror("fork");
		exit(1);
		break;
	case 0://child process
		printf("-->childe process\n");
		exit(2);
		break;
	default://parent process
		while(wait(&status) != pid) continue;
		printf("-->parent process\n");
		printf("status: %d, %x\n",status, status);
		printf("childe process exit status: %d\n", WEXITSTATUS(status));
		break;

	}

	return 0;
}
