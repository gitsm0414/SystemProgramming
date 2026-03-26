#include<time.h>
#include<stdio.h>

int main(void){
	struct tm* tm;
	time_t t;

	time(&t);
	printf("Time(sec): %d\n", (int)t);

	tm = gmtime(&t);
	printf("Y: %d ", tm->tm_year);
	printf("M: %d ", tm->tm_mon);
	printf("D: %d ", tm->tm_mday);
	printf("H: %d ", tm->tm_hour);
	printf("M: %d ", tm->tm_min);
	printf("S: %d ", tm->tm_sec);


	return 0;
}
