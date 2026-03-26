#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    const char *path = ".";
    int fd;

    printf("========== Path Configuration (%s) ==========\n", path);

    // 1. pathconf 사용 (경로명 기반)
    printf("Max File Name Length:  %ld\n", pathconf(path, _PC_NAME_MAX));
    printf("Max Path Length:       %ld\n", pathconf(path, _PC_PATH_MAX));
    printf("Max Hard Links:        %ld\n", pathconf(path, _PC_LINK_MAX));

    // 2. fpathconf 사용 (파일 디스크립터 기반)
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("\n--- Using File Descriptor ---\n");
    // chown 제한 여부 (보통 리눅스는 1(참) 반환)
    printf("Chown Restricted:      %ld\n", fpathconf(fd, _PC_CHOWN_RESTRICTED));
    // 이름 잘림 방지 여부
    printf("No Truncate Long Name: %ld\n", fpathconf(fd, _PC_NO_TRUNC));
    // 파이프 버퍼 크기
    printf("Pipe Buffer Size:      %ld\n", fpathconf(fd, _PC_PIPE_BUF));

    close(fd);
    printf("=============================================\n");

    return 0;
}
