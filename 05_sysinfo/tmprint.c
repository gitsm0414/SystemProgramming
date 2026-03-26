#include <stdio.h>
#include <time.h>

int main() {
    time_t t;
    struct tm *tm_ptr;
    char buf[128];

    // 1. 현재 시간 가져오기
    t = time(NULL);
    tm_ptr = localtime(&t);

    printf("========== Time Printing Methods ==========\n");

    // 2. ctime() 사용 (time_t -> string)
    // 끝에 \n이 자동으로 포함되어 출력됩니다.
    printf("1. ctime()    : %s", ctime(&t));

    // 3. asctime() 사용 (struct tm -> string)
    printf("2. asctime()  : %s", asctime(tm_ptr));

    // 4. strftime() 사용 (Custom Format)
    // 형식: [2026년 03월 27일 00시 05분]
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S (%A)", tm_ptr);
    printf("3. strftime() : %s\n", buf);

    // 로그 스타일 포맷 예시
    strftime(buf, sizeof(buf), "[%y/%m/%d %H:%M]", tm_ptr);
    printf("4. Log Style  : %s\n", buf);

    printf("==========================================\n");

    return 0;
}
