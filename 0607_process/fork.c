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
		printf("child process: my PID: %d, my PPID: %d\n", (int)getpid(), (int)getppid());
		break;
	default:
		printf("parent process: my PID: %d, my PPID: %d, my child's PID: %d\n", (int)getpid(), (int)getppid(), pid);
		break;

	}
	printf("end of fork\n");
	return 0;
}
