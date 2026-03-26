#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>


int main(void){
	struct stat buf;

	chmod("linux.txt", S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH); //0754
	stat("linux.txt", &buf);
	printf("permission change 1 = %o\n", (unsigned int)buf.st_mode);

	buf.st_mode |= S_IWGRP;
	buf.st_mode &= ~(S_IROTH);
	//0770
	
	chmod("linux.txt", buf.st_mode);
	stat("linux.txt", &buf);
	printf("permission change 2 = %o\n", (unsigned int)buf.st_mode);

	return 0;
}
