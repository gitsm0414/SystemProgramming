#include <stdio.h>
#include <pwd.h>    // passwd 구조체 및 관련 함수 헤더
#include <unistd.h> // getuid() 함수 헤더
#include <sys/types.h>

int main(void) {
    struct passwd *pw;
    uid_t uid;

    // 1. 현재 프로세스를 실행 중인 사용자의 UID를 가져옵니다.
    uid = getuid();

    // 2. 해당 UID를 가진 사용자의 정보를 passwd 구조체 형태로 가져옵니다.
    pw = getpwuid(uid);

    if (pw == NULL) {
        perror("getpwuid 호출 실패");
        return 1;
    }

    // 3. 구조체의 모든 멤버를 출력합니다.
    printf("========== User Passwd Info ==========\n");
    printf("1. User Name (pw_name)   : %s\n", pw->pw_name);
    printf("2. Password  (pw_passwd) : %s\n", pw->pw_passwd); // 보통 'x' 출력
    printf("3. User ID   (pw_uid)    : %d\n", (int)pw->pw_uid);
    printf("4. Group ID  (pw_gid)    : %d\n", (int)pw->pw_gid);
    printf("5. User Info (pw_gecos)  : %s\n", pw->pw_gecos);
    printf("6. Home Dir  (pw_dir)    : %s\n", pw->pw_dir);
    printf("7. Login Shell(pw_shell) : %s\n", pw->pw_shell);
    printf("======================================\n");

    return 0;
}
