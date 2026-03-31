#include<stdlib.h>
#include<stdio.h>

void cleanup1(void){
	printf("cleanup1 is called\n");
}
void cleanup2(void){
	printf("cleanup2 is called\n");
}


int main(void){
	atexit(cleanup1);
	atexit(cleanup2);	

	exit(0);
}
