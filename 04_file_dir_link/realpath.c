#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#define PRINT_ERR_EXIT(_msg) {perror(_msg); exit(1);}

int main(void){
    char buf[BUFSIZ];
    int n;

    n = readlink("linux.sym", buf, BUFSIZ); //알아서 '\0'을 넣어주지 않는다.
    if(n == -1) PRINT_ERR_EXIT("readlink");

    buf[n] = '\0';
    printf("linux.sym: readlink = %s\n", buf);

    realpath("linux.sym", buf); //알아서 '\0'을 넣어준다.
    printf("linux.sym: realpath = %s\n", buf);

    return 0;
}