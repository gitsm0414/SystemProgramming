#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main() {
    time_t t;
    struct timeval tv;
    
    // 1. time() 함수: 단순 초 단위 (Epoch Time)
    t = time(NULL);
    printf("1. Current Epoch Time (seconds): %ld\n", (long)t);

    // 2. gettimeofday() 함수: 마이크로초 단위 정밀도
    if (gettimeofday(&tv, NULL) == 0) {
        printf("2. Precise Time: %ld sec, %ld micro-sec\n", 
                tv.tv_sec, (long)tv.tv_usec);
    }

    // 3. tzset() 및 전역 변수: 시간대 정보 확인
    tzset(); // 환경 변수 TZ를 기반으로 전역 변수 설정
    
    printf("\n--- Timezone Information ---\n");
    printf("Timezone Name: %s, %s\n", tzname[0], tzname[1]);
    
    // timezone 전역 변수는 UTC와의 차이를 '초' 단위로 나타냄
    // 한국(KST)은 UTC+9 이므로 -32400 (9 * 3600)이 출력됨 (서쪽이 +, 동쪽이 - 기준)
    printf("Difference from UTC: %ld hours\n", -timezone / 3600);
    printf("Daylight: %d\n", daylight);

    // 4. 간단한 실행 시간 측정 실습
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    printf("\nProcessing... (sleeping for 1 second)\n");
    sleep(1); 
    
    gettimeofday(&end, NULL);
    
    long diff_sec = end.tv_sec - start.tv_sec;
    long diff_usec = end.tv_usec - start.tv_usec;
    
    if (diff_usec < 0) { // 마이크로초 보정
        diff_sec--;
        diff_usec += 1000000;
    }
    
    printf("Measured Elapsed Time: %ld.%06ld seconds\n", diff_sec, diff_usec);

    return 0;
}
