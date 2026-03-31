#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>

#define PRINT_ERR_EXIT(_msg) {perror(_msg); exit(1);}


int main(void){
	DIR* dp;
	struct dirent *dent;

	if((dp = opendir(".")) == NULL)

		PRINT_ERR_EXIT("open dir");

	printf("** Directory content **\n");
	printf("Start Offset: %ld\n", telldir(dp));

	while((dent =readdir(dp))){
		printf("Read: %s ", dent->d_name);
		printf("Cur offset: %ld\n", telldir(dp));
		//여기서 cur offset은 다음으로 읽어들일 entry의 offset값이다.
	}

	printf("** Directory pointer rewind **\n");
	rewinddir(dp);
	printf("Cur offset: %ld\n", telldir(dp));

	for(int i = 0; i < 2; i++)
		readdir(dp); //처음위치부터 두개의 엔트리를 건너뛴다. 
	
	printf("** Move directory pointer **\n");
	
	printf("Cur offset: %ld\n", telldir(dp));

	dent = readdir(dp); 
	printf("Read: %s ", dent->d_name);
	printf("Next offset: %ld\n", telldir(dp));

	closedir(dp);

	return 0;
}
