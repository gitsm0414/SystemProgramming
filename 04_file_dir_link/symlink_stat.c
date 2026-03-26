#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>

#define PRINT_CNT_INODE {\
printf("linux.txt: link count = %d\n", (int)buf.st_nlink);\
printf("linux.txt: Inode = %d\n", (int)buf.st_ino);}


int main(void){
    struct stat buf;
    
    printf("1. stat: linux.txt ---\n");
    stat("linux.txt", &buf);
    PRINT_CNT_INODE;

    printf("2. stat: linux.sym ---\n");
    stat("linux.sym", &buf); //최종목적 파일인 linux.txt를 읽어온다.
    PRINT_CNT_INODE;

    printf("3. lstat: linux.sym ---\n");
    lstat("linux.sym", &buf); //symbolic link file자체를 읽어온다.
    PRINT_CNT_INODE;

    return 0;
}