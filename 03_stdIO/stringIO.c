#include<stdio.h>
#include<stdlib.h>


int main(void){
	FILE* rfp, * afp;
	char buf[BUFSIZ]; // BUFSIZ is defined in stdio.h

	printf("BUFSIZ: %d\n", BUFSIZ);

	if((rfp = fopen("hello.txt", "r")) == NULL){
		perror("fopen hello.txt");
		exit(1);
	}

	if((afp = fopen("hello.out", "a")) == NULL){
		perror("fopen hello.out");
		exit(1);
	}

	while(fgets(buf, BUFSIZ, rfp) != NULL){
		fputs(buf,afp);
	}

	fclose(rfp);
	fclose(afp);

	return 0;
}
