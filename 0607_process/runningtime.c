#include<sys/types.h>
#include<sys/times.h>
#include<time.h>
#include<limits.h>//시스템 제한사항들이 정의되어있다. 여기선 _SC_CLK_TCK를 이용
#include<unistd.h>//sysconf(), sleep()
#include<stdlib.h>//exit()
#include<stdio.h>//printf(), perror()

int main(void){
	time_t t;
	struct tms tms1, tms2;
	clock_t t1, t2;
	long clock_ticks = sysconf(_SC_CLK_TCK);

	if((t1 = times(&tms1)) == -1){ perror("times 1"); exit(1);}

	sleep(2);//sleep()은 cpu를 쓰지 않아서 user time, system time에는 포함되지 않는다.
	//2초 동안 process는 실행되지않고 멈춘다.
	for(int i = 0; i < 9999999; i++) time(&t);

	if((t2 = times(&tms2)) == -1){ perror("times 2"); exit(1);}
	
	printf("Real time: %.3f sec\n", (double)(t2-t1)/clock_ticks);
	printf("User time: %.3f sec\n", (double)(tms2.tms_utime - tms1.tms_utime)/clock_ticks);
	printf("System time: %.3f sec\n", (double)(tms2.tms_stime - tms1.tms_stime)/clock_ticks);

	return 0;
}

