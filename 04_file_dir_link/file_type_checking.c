#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>

int main(void){
	struct stat buf;
	int kind;

	stat("linux.txt", &buf); //&buf로 보내는 점 유의
	
	printf("Mode = %o (Hexa: %x)\n", (unsigned int)buf.st_mode, (unsigned int)buf.st_mode);

	kind = buf.st_mode & S_IFMT; // buf.st_mode (&) (1111 0000 0000 0000)
	
	printf("kind = %x\n", kind);
	
	switch(kind){
		case S_IFIFO:
			printf("linux.txt: FIFO\n"); break;
		case S_IFDIR:
			printf("linux.txt: Directory\n"); break;
		case S_IFREG:
			printf("linux.txt: Regular file\n"); break;
	}

	//아래는 macro function을 이용한 방법
	
	if(S_ISFIFO(buf.st_mode)) printf("linux.txt: FIFO\n");
	if(S_ISDIR(buf.st_mode)) printf("linux.txt: DIR\n");
	if(S_ISREG(buf.st_mode)) printf("linux.txt: REG\n");

	return 0;
}
