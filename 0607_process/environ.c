#include<stdlib.h>
#include<stdio.h>
/*
보통 이렇게 extern 전역변수를 활용하는게 호환성이 좋다.
extern char **environ;

int main(void){
    char **env;
    env = environ;

    while(*env){
        printf("%s\n", *env);
        env++;
    }

    return 0;
}
*/
int main(int argc, char **argv, char **envp){//이렇게 메인함수의 인자로 넘어가게할수도있다. 그냥 ./[실행파일] 하면된다.
    char** env;
    env = envp;

    while(*env){
        printf("%s\n", *env);
        env++;
    }

    return 0;   
}