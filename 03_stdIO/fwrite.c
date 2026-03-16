#include<stdio.h>
#include<stdlib.h>


int main(void){
	FILE* fp = NULL;
	char* filename = "binary.bin";
	int data[5] = {1, 2, 3, 4, 5};

	if( !(fp = fopen(filename, "wb")) ){
		fprintf(stderr, "Fail to open the file - %s\n", filename);
		exit(1);
	}

	size_t i = fwrite(data, sizeof(int), 5, fp);
	printf("Success to write %d object(s).\n", (int)i);

	fclose(fp);
	return 0;

}
