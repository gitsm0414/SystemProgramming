#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void) {
    struct stat buf;

    // "stat.c" 파일의 정보를 읽어 buf 구조체에 저장합니다.
    stat("stat.c", &buf);

    printf("Inode = %d\n", (int)buf.st_ino);
    printf("Mode = %o\n", (unsigned int)buf.st_mode); //mode값은 8진수로 표현하는것이 관례
    printf("Nlink = %lu\n", (unsigned long)buf.st_nlink);
    printf("UID = %d\n", (int)buf.st_uid);
    printf("GID = %d\n", (int)buf.st_gid);
    printf("SIZE = %d\n", (int)buf.st_size);
    printf("Atime = %d\n", (int)buf.st_atime);
    printf("Mtime = %d\n", (int)buf.st_mtime);
    printf("Ctime = %d\n", (int)buf.st_ctime);
    printf("Blksize = %d\n", (int)buf.st_blksize);
    printf("Blocks = %d\n", (int)buf.st_blocks);

    return 0;
}