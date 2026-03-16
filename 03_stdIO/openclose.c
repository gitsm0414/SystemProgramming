#include<stdio.h>
#include<stdlib.h>


int main(void){
	FILE *fp;

	if((fp = fopen("hello.txt", "w")) == NULL){
		perror("hello.txt");
		exit(1);
	}

	fclose(fp);

	return 0;
}
