#include<stdlib.h>
#include<stdio.h>

int main(void){
    char *val;

    val = getenv("SHELL"); //환경변수는 이미 어딘가에 저장되어 있고 그 스트링의 시작주소만 복사해오는 것이다.
    //따라서 메모리할당이나 free()걱정할 필요는 없다.
    if(val == NULL){
        printf("SHELL is not defined\n");
    }else{
        printf("SHELL = %s\n", val);
    }

    return 0;
}