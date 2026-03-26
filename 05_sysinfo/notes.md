<unix/linux system information>
1. 운영체제에 대한 정보
2. 호스트명 정보(=~컴퓨터 이름)
3. 하드웨어 종류 정보
4. 하드웨어에 따라 사용할 수 있는 자원의 최댓값(프로세스 최대 개수, 프로세스당 최대 파일 개수, 메모리 페이지 크기 등)


<uname(1)>
시스템의 기본 정보를 출력
-a: 전체 정보
Linux DESKTOP-00C6I1S 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
운영체제명, 호스트명, 릴리즈레벨, 버전번호 ...

이 외의 옵션들은 man -s 1 uname 참고

<uname(2)>
#include<sys/utsname.h>

    int uname(struct utsname *buf); //-1 error

>>활용은 uname.c 참고

<sysinfo(2)>
#include<sys/sysinfo.h>

    int sysinfo(struct sysinfo *info); //-1 error

struct sysinfo{
    long    uptime;             //seconds since boot
    unsigned long   loads[3];   //1분, 5분, 15분마다 평균 부하량
    unsigned long   totalram;   //사용가능한 총 메인메모리 사이즈
    unsigned long   freeram;    //사용가능한 메모리 사이즈
    unsigned long   sharedram;  //
    unsigned long   bufferram;  //
    unsigned long   totalswap;  //
    unsigned long   freeswap;   //
    unsigned short  proc;       // 현재 프로세스 수
    char _f[22];                //
};


<sysconf(3)>: 시스템의 resource 정보를 얻어온다. cf. configuration: 설정
#include<unistd.h>

    long sysconf(int name); //요청한 정보의 값, -1 error

name: 검색할 정보를 지칭하는 상수, 시스템에 따라 다르니 man 페이지 확인

① 프로세서 및 하드웨어 정보
    _SC_NPROCESSORS_CONF: 시스템에 구성된 총 프로세서(코어) 수.

    _SC_NPROCESSORS_ONLN: 현재 온라인(사용 가능) 상태인 프로세서 수.

    _SC_PAGESIZE: 시스템의 메모리 페이지 크기 (바이트 단위). mmap 등을 쓸 때 중요합니다.

② 리소스 제한 정보
    _SC_OPEN_MAX: 한 프로세스가 동시에 열 수 있는 파일의 최대 개수.

    _SC_CHILD_MAX: 한 사용자가 동시에 실행할 수 있는 프로세스의 최대 개수.

    _SC_ARG_MAX: exec() 계열 함수로 전달할 수 있는 인자(argv + envp)의 최대 길이.

③ 시간 및 클록 정보
    _SC_CLK_TCK: 초당 클록 틱 수. times() 함수가 반환하는 값을 초 단위로 바꿀 때 사용합니다.


<파일, 디렉토리 conf>
#include<unistd.h>: man -s 3

    long fpathconf(int fd, int name);

    long pathconf(const char* path, int name);

파일이나 디렉토리의 리소스정보를 얻을 때 사용한다.

① 파일 이름 및 경로 제한
    _PC_NAME_MAX: 해당 디렉토리 내에서 만들 수 있는 파일 이름의 최대 길이.

    _PC_PATH_MAX: 해당 파일/디렉토리를 포함한 전체 경로명의 최대 길이.

② 링크 및 입출력 제한
    _PC_LINK_MAX: 하나의 파일에 생성할 수 있는 하드 링크의 최대 개수.

    _PC_PIPE_BUF: 파이프(Pipe)에 데이터를 쓸 때, 원자적(Atomic)으로 써지는 것이 보장되는 최대 바이트 수.

    _PC_MAX_CANON: 터미널의 정규 입력 줄(Canonical input line)의 최대 길이.

③ 보안 및 특성
    _PC_CHOWN_RESTRICTED: chown() 함수 사용 시 권한 제한이 있는지 여부.

    _PC_NO_TRUNC: 파일 이름이 _PC_NAME_MAX보다 길 때, 에러를 낼 것인지 아니면 이름을 잘라버릴 것인지 여부.


<유저 추가 등등>
sudo adduser [username]

su [username] (su: switch user)
exit 로 나갈 수 있다.

<userID & login name>: 사용자에게 부여된 아이디번호, 문자형태의 사용자이름이다. 서로 매칭된다.
$id
uid=1000(tjtjdals) gid=1000(tjtjdals) groups=1000(tjtjdals),4(adm),20(dialout),24(cdrom),25(floppy),27(sudo),29(audio),30(dip),44(video),46(plugdev),100(users),107(netdev)

$id sp2026 (새로만든 유저, 비번 동일)
uid=1001(sp2026) gid=1001(sp2026) groups=1001(sp2026),100(users)
$

<userID & login name 프로세스 입장에서>
real user ID: 최초에 process를 실행한 유저의 ID
effective user ID(EUID): 현재 process가 행사하는 UID, process가 가진 권한 판단에 사용됨
saved user ID: process최초의 EUID를 저장. 다시 돌아가기 위해 사용


<get/set user id>:man -s 2
#include<sys/types.h>
#include<unistd.h>

    uid_t getuid(void); //현재 프로세스의 real userID
    uid_t geteuid(void); //현재 프로세스의 EUID

    int setuid(uid_t uid); //어떠한 uid를 EUID로 설정해준다. 일반사용자는 real UID 혹은 saved UID 설정만 가능

>>uid.c 참고
$ ls -l uid.out
-rwxr-xr-x 1 tjtjdals tjtjdals 17680 Mar 26 21:13 uid.out
$ ./uid.out >>RUID, EUID 모두 소유자인 1000으로 설정된 모습
[init] UID = 1000, EUID = 1000
[seteuid(uid)] UID = 1000, EUID = 1000
[seteuid(euid)] UID = 1000, EUID = 1000

$ sudo ./uid.out >> RUID, EUID 모두 관리자인 0으로 설정된 모습
[init] UID = 0, EUID = 0
[seteuid(uid)] UID = 0, EUID = 0
[seteuid(euid)] UID = 0, EUID = 0

$ chmod 4755 uid.out >> 권한 16비트 중 setuid = 1로 설정한 모습(0755 -> 4755) 
$ ls -l uid.out
-rwsr-xr-x 1 tjtjdals tjtjdals 17680 Mar 26 21:13 uid.out >> -rwx..가 아니라 -rws가 됐다.

$ sudo ./uid.out >> RUID는 관리자지만, chmod로 인해 EUID는 소유자(1000)으로 설정된 모습
[init] UID = 0, EUID = 1000
[seteuid(uid)] UID = 0, EUID = 0
[seteuid(euid)] UID = 0, EUID = 1000


<user information>
/etc/passwd (파일) >> 사용자에 대한 기본정보가 들어있는 파일

/etc/shadow (파일) >> 암호화된 비번이 들어있는 파일
tjtjdals@DESKTOP-00C6I1S:/etc$ sudo cat shadow | grep tjtjdals >> 암호화된 상태로 문자들이 나타남

<reading passwd file>: man -s 3
#include<sys/types.h>
#include<pwd.h>

    struct passwd* getpwnam(const char* name);
    struct passwd* getpwuid(uid_t uid);

struct passwd {
    char   *pw_name;       /* 사용자 로그인 이름 (Login Name) */
    char   *pw_passwd;     /* 암호 (보통 'x'로 표시됨, 실제 암호는 /etc/shadow에 있음) */
    uid_t   pw_uid;        /* 사용자 ID (User ID) */
    gid_t   pw_gid;        /* 그룹 ID (Group ID) */
    char   *pw_gecos;      /* 사용자 정보 (이름, 연락처 등 상세 정보) */
    char   *pw_dir;        /* 홈 디렉토리 경로 (Home Directory) */
    char   *pw_shell;      /* 로그인 쉘 경로 (Login Shell) */
};


#include<shadow.h>

    struct spwd* getspnam(const char* name);

    //shadow파일에서 사용자정보를 순차적으로 읽어옴
    struct spwd* getspent(void);
    void setspent(void);
    void endspent(void);


<group ID>
-real group ID
-effective group ID
-saved group ID

getgid(), getegid(), setegid() >>man -s 2 ...

#include<sys/types.h>
#include<grp.h>

    struct group* getgrnam(const char* name);
    struct group* getgrgid(gid_t gid);

    struct group* getgrent(void);
    void setgrent(void);
    void endgrent(void);


struct group {
    char   *gr_name;      /* 그룹 이름 */
    char   *gr_passwd;    /* 그룹 암호 (보통 사용 안 함) */
    gid_t   gr_gid;       /* 그룹 ID (숫자) */
    char  **gr_mem;       /* 그룹에 속한 사용자 이름들의 리스트 (NULL로 끝남) */
};


<time>
UTC: 유닉스에서는 1970년 1월 1일 00:00:00를 기준으로 경과한 시간을 초단위로 저장한다.

#include<sys/time.h>:man -s 2
    time_t time(time_t* tloc);//리턴: 얻어온 초, 에러 -1

#include<sys/time.h>:man -s 3

    int gettimeofday(struct timeval* tv, struct timezone* tz);
    int settimeofday(const struct timeval* tv, struct timezone* tz); //root 권한 필요

기준 시간으로부터 경과한 시간을 micro-second(10^-1)단위로 읽고/씀.
tv는 일반적으로 NULL사용, 자세한 내용은 man 참조
성공 0, 실패 -1

struct timeval{
    time_t      tv_sec; //초
    suseconds_t tv_usec;//마이크로 초
};

#include<time.h>:man -s 3

    void tzset(void); //현재 지역의 시간대로 설정함

관련정보는 전역변수에 저장돼있음

extern char *tzname[2]; //time zone name
extern long timezone; //UTC(국제표준시)와 시차(초)
extern int daylight; //DST(일광시?)적용 여부(0: 적용)

<tm ~ seconds>
#include<time.h>

struct tm {
   int tm_sec;         /* 초 (0-60) */
   int tm_min;         /* 분 (0-59) */
   int tm_hour;        /* 시 (0-23) */
   int tm_mday;        /* 일 (1-31) */
   int tm_mon;         /* 월 (0-11) -> 0이 1월입니다! */
   int tm_year;        /* 년 (1900년 이후 경과 년수) */
   int tm_wday;        /* 요일 (0-6) -> 0이 일요일입니다. */
   int tm_yday;        /* 올해 1월 1일부터 경과된 일수 (0-365) */
   int tm_isdst;       /* 일광 절약 시간제(DST) 적용 여부 */
};

    struct tm* gmtime(const time_t* timep); //UTC 기준으로 초를 tm으로 변환
    struct tm* localtime(const time_t* timep); //Local time 기준으로 초를 tm으로 변환

    time_t mktime(struct tm* tm); //tm을 초로 변환


<printing time>
#include<time.h>

char* ctime(const time* timep);
char* asctime(const struct tm* tm);
>>Sat Jan 31 11:59:19 2009 처럼 보기좋게 글자형식으로 바꿔주는 함수

size_t strftime(char* s, size_t max, const char* format, const struct tm* tm);
>>원하는 포맷(예: 2026-03-27 00:05)을 직접 지정할 수 있는 가장 강력한 함수입니다.

주요 포맷 상수:
%Y: 4자리 년도 (2026)
%m: 월 (01~12)
%d: 일 (01~31)
%H: 24시간 형식 (00~23)
%M: 분 (00~59)
%S: 초 (00~60)
이외에도 많은데 나머지는 man page 참조