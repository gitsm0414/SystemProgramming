#include <stdio.h>
#include <shadow.h>
#include <stdlib.h>
#include <errno.h>

void print_shadow_entry(struct spwd *sp) {
    if (sp == NULL) return;
    
    printf("User Name          : %s\n", sp->sp_namp);
    printf("Password Hash      : %s\n", sp->sp_pwdp); // 암호 해시값
    printf("Last Change (days) : %ld\n", sp->sp_lstchg); // 마지막 변경일 (1970-01-01 기준 일수)
    printf("Min Days to Change : %ld\n", sp->sp_min);
    printf("Max Days to Change : %ld\n", sp->sp_max);
    printf("Warning Days       : %ld\n", sp->sp_warn);
    printf("----------------------------------------\n");
}

int main(int argc, char *argv[]) {
    struct spwd *sp;

    // 1. 특정 사용자 정보 가져오기 (getspnam)
    if (argc > 1) {
        printf("--- Searching for user: %s ---\n", argv[1]);
        sp = getspnam(argv[1]);
        if (sp == NULL) {
            if (errno == EACCES) printf("Error: Permission denied (Use sudo!)\n");
            else printf("User not found.\n");
        } else {
            print_shadow_entry(sp);
        }
    }

    // 2. 전체 사용자 순회하기 (getspent)
    printf("\n--- Listing all shadow entries ---\n");
    setspent(); // 파일의 처음으로 포인터 이동
    
    while ((sp = getspent()) != NULL) {
        printf("[%s] ", sp->sp_namp);
    }
    printf("\n");

    endspent(); // 파일 닫기
    return 0;
}
