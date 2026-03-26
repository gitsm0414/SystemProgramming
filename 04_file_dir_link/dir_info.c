#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>


int main(){
	DIR* dp;
	struct dirent *dent;

	if((dp = opendir(".")) == NULL){
		perror("opendir: .");
		exit(1);
	}

	while((dent = readdir(dp))){
		printf("Name: %s ", dent->d_name);
		printf("Inode: %d\n", (int)dent->d_ino);
	}

	closedir(dp);

	return 0;
}
