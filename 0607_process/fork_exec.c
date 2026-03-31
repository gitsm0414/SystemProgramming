#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>


int main(void){
	pid_t pid;

	switch(pid = fork()){
	case -1:
		perror("fork"); exit(1);
		break;
	case 0:
		printf("-->child process\n");
		if(execlp("ls", "ls", "-a", (char *)NULL) == -1){
			perror("execlp");
			exit(1);
		}
		exit(0);
		break;
	default:
		printf("-->parent process - my pid: %d\n", (int)getpid());
		break;

	}
	printf("end of fork\n");
	return 0;
}

