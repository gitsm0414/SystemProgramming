#include <stdio.h>
#include <grp.h>
#include <unistd.h>
#include <sys/types.h>

void print_group_info(struct group *gr) {
    if (gr == NULL) return;
    printf("Group Name: %s (GID: %d)\n", gr->gr_name, (int)gr->gr_gid);
    printf("Members: ");
    for (int i = 0; gr->gr_mem[i] != NULL; i++) {
        printf("%s%s", gr->gr_mem[i], (gr->gr_mem[i+1] != NULL) ? ", " : "");
    }
    printf("\n----------------------------------------\n");
}

int main() {
    struct group *gr;
    gid_t rgid, egid;

    // 1. 현재 내 프로세스의 GID 확인
    rgid = getgid();  // Real GID
    egid = getegid(); // Effective GID

    printf("My Real GID: %d\n", (int)rgid);
    printf("My Effective GID: %d\n\n", (int)egid);

    // 2. 내 실제 그룹 이름 찾아보기
    printf("--- My Group Details ---\n");
    gr = getgrgid(rgid);
    print_group_info(gr);

    // 3. 시스템의 모든 그룹 순회하며 출력 (상위 5개만)
    printf("--- System Groups (First 5) ---\n");
    setgrent(); // 파일 처음으로
    for (int i = 0; i < 5; i++) {
        gr = getgrent();
        if (gr == NULL) break;
        printf("[%d] %s\n", i+1, gr->gr_name);
    }
    endgrent();

    return 0;
}
