#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#define PRINT_ERROR_EXIT(_msg) {perror(_msg); exit(1);}

int main(){
	if(mkdir("dir_a", 0755) == -1)
		PRINT_ERROR_EXIT("dir_a");

	if(mkdir("dir_b", 0755) == -1)
		PRINT_ERROR_EXIT("dir_b");
	
	if(rename("dir_a", "new_a") == -1)
		PRINT_ERROR_EXIT("new_a");
	
	if(rmdir("dir_b") == -1)
		PRINT_ERROR_EXIT("rm_dir_b");
	//현재 new_a만 있는상태
	
	char* cwd;
	char wd[BUFSIZ]; 
	
	cwd = getcwd(NULL, BUFSIZ);
	printf("1. Current directory: %s\n", cwd);

	chdir("new_a"); //절대경로도 되고 상대경로도 된다.
	
	getcwd(wd, BUFSIZ);
	printf("2. Current directory: %s\n", wd);
	//current working directory가 new_a로 바뀌었다.

	return 0;
}
