#include<sys/errno.h>
#include<unistd.h>
#include<stdio.h>

extern int errno;

int main(void){
	int per;

	//linux.bak이란 파일의 존재유무 확인
	if(access("linux.bak", F_OK) == -1 && errno == ENOENT){
		printf("linux.bak: file does not exist.\n");
	}

	//linux.txt 읽기 권한 확인
	per = access("linux.txt", R_OK);
	if(per == 0){
		printf("linux.txt: read permitted.\n");
	}else if(per == -1 && errno == EACCES){
		printf("linux.txt: read not permitted.\n");
	}

	return 0;
}
