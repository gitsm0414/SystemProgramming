#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    printf("========== System Configuration ==========\n");

    // 1. CPU 정보
    printf("CPU Cores (Configured): %ld\n", sysconf(_SC_NPROCESSORS_CONF));
    printf("CPU Cores (Online):     %ld\n", sysconf(_SC_NPROCESSORS_ONLN));

    // 2. 메모리 페이지 크기
    printf("Memory Page Size:       %ld bytes\n", sysconf(_SC_PAGESIZE));

    // 3. 파일 및 프로세스 제한
    printf("Max Open Files:         %ld\n", sysconf(_SC_OPEN_MAX));
    printf("Max Child Processes:    %ld\n", sysconf(_SC_CHILD_MAX));

    // 4. 인자 최대 길이
    printf("Max Arg Length:         %ld bytes\n", sysconf(_SC_ARG_MAX));

    // 5. 클록 틱 (1초에 몇 번 틱이 발생하는가)
    printf("Clock Ticks per sec:    %ld\n", sysconf(_SC_CLK_TCK));

    printf("==========================================\n");

    return 0;
}
