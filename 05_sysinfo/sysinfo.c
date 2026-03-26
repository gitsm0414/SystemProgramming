#include <stdio.h>
#include <sys/sysinfo.h> // sysinfo 구조체와 함수가 정의된 헤더

int main() {
    struct sysinfo info;

    // sysinfo 시스템 콜 호출
    if (sysinfo(&info) != 0) {
        perror("sysinfo 호출 실패");
        return 1;
    }

    // 1. 부팅 후 경과 시간 (uptime)
    long days = info.uptime / (24 * 3600);
    long hours = (info.uptime % (24 * 3600)) / 3600;
    long minutes = (info.uptime % 3600) / 60;

    printf("** System Statistics **\n");
    printf("Uptime: %ld days, %ld hours, %ld minutes (%ld seconds)\n", 
            days, hours, minutes, info.uptime);

    // 2. 프로세스 개수
    printf("Total Processes: %hu\n", info.procs);

    // 3. 메모리 정보 (Byte 단위를 MB 단위로 변환)
    // 최근 리눅스 커널에서는 메모리 값에 info.mem_unit을 곱해줘야 정확한 바이트가 나옵니다.
    unsigned long long total_mem = (unsigned long long)info.totalram * info.mem_unit;
    unsigned long long free_mem = (unsigned long long)info.freeram * info.mem_unit;

    printf("Total RAM: %llu MB\n", total_mem / (1024 * 1024));
    printf("Free RAM: %llu MB\n", free_mem / (1024 * 1024));

    // 4. 로드 애버리지 (1분, 5분, 15분 평균 부하)
    // loads 값은 65536으로 나눠야 우리가 아는 소수점 형태의 부하 수치가 나옵니다.
    printf("Load Average: 1min(%.2f), 5min(%.2f), 15min(%.2f)\n", 
            (float)info.loads[0] / 65536, 
            (float)info.loads[1] / 65536, 
            (float)info.loads[2] / 65536);

    return 0;
}
