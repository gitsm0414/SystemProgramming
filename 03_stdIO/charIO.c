#include<stdio.h>
#include<stdlib.h>


int main(void){
	FILE* rfp, * wfp;
	int c;

	if((rfp = fopen("hello.txt", "r")) == NULL){
		perror("fopen: hello.txt");
		exit(1);
	}

	if((wfp = fopen("hello.out", "w")) == NULL){
		perror("fopen: hello.out");
		exit(1);
	}

	while((c = fgetc(rfp)) != EOF){ //remember that c is integer type
		fputc(c, wfp);
	}

	fclose(rfp);
	fclose(wfp);

	return 0;
}
