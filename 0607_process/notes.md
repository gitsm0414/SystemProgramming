<process>
프로그램: 아직 실행되지 않은 프로그램 + 데이터
프로세스: 실행중인 프로그램. 사용자가 프로그램을 실행하면 커널이 이를 등록하여 관리한다.

프로세스 할당 메모리
1. heap memory: dynamic memory가 저장되는 공간, 아래로 확장
2. stack memory(auto variable):지역변수가 저장되는 공간
3. data segment(static variable):스태틱변수, 전역변수 저장되는 공간(프로그램실행~종료까지 위치와 크기가 고정됨)
4. text/code segment: 코드가 저장되는 공간

1, 2: 가변영역
3, 4: 고정영역

<shell commands>
ps: 사용자가 실행한 프로세스들 정보확인
-u username
-j: job control format으로 출력(상세히 출력)
-e: every >> 사용자 뿐만 아니라 시스템의 모든 process 출력

top: 윈도우로 치면 작업관리자
현재 시스템 내 프로세스 정보 실시간 확인

kill -signal pid: 프로세스에게 signal을 보내는 명령
예를들어 -9는 프로세스 강제종료


<process ID>
각 프로세스에 유일하게 할당된 번호

부모 프로세스: 모든 프로세스는 부모가 있다. 최상위 부모는 kernel이다.(PID = 0)
부모의 PID를 PPID(parent process ID)라 한다.

User ID vs Process ID vs UID of a process 이 세가지를 구분할 줄 알아야한다.

<PID>: man -s 2
#include<unistd.h>
#include<sys/types.h>

    pid_t getpid(void);
    pid_t getppid(void);

<process group>
하나의 job수행을 목적으로 하는 process들
예: shell script의 명령어들, 부모와 자식 프로세스 등

process group에게 전달된 signal은 그룹원 모두에게 전달됨.
>>job단위 프로세스 제어를 위해 활용

process group에는 leader가 존재한다.
PGID: group leader의 PID

특정 그룹의 리더의 PID가 pid라 하자. 즉 pid는 PID이자 PGID이다.
kill -9 pid를 하면 그룹에서 리더만 죽고 나머진 영향이 없다. 하지만
kill -9 -pid를 하면, -뒷부분을 PGID로 인식하여 해당 그룹 모두를 강제종료한다.


<PGID>:man -s 2
#include<unistd.h>

    pid_t getpgid(pid_t pid); //pid의 PGID를 리턴한다. pid = 0인경우 현재 이함수를 실행중인 나(self)의 PGID를 알려준다.
    pid_t getpgrp(void); //무조건 얘를 호출한 자신(self)의 PGID를 반환한다. >> getpgid(0) 동일

따라서 if(getpgid(pid) == pid) 참이면 pid는 리더이다.

    pid_t setpgid(pid_t pid, pid_t pgid); //pid process를 pgid 그룹으로 옮긴다.
>> 그룹 이동은 같은 session내에서만 가능
>> pid = 0 이면 current process를 의미
>> pgid = 0 이면 pgid를 pid로 설정. 즉 "내가 리더가 되겠다"

ex) setpgid(0, 0): "나 지금부터 내 PID를 번호로 하는 새 팀 만든다! 내가 팀장이다!"
ex) setpgid(자식PID, 부모PGID): "너(자식)는 딴 맘 품지 말고 우리 팀(부모 그룹)에 있어라." + 보통 부모 프로세스가 fork()를 한 직후에, 부모와 자식 양쪽 모두에서 setpgid()를 호출하는 것이 관례


<session>
사용자가 로그인 시 새 sessin이 생성됨(새 터미널 창)
세션 안에 여러 그룹이 있고, 그룹리더들 중 하나는 그룹리더이자 세션리더이기도 하다.
세션은 process group과 inturrupt unit(키보드..)를 연결하는 방법이다.(현재 주시하는 창 쪽으로 연결해준다.)

세션은 1개의 foreground process group과 0개 이상의 background process group을 가진다.
foreground process group: 사용자와 I/O를 현재 수행하는 녀석. 터미널을 통해 전달되는 signal을 받는다.
background process group: 나머지. 그래도 output은 터미널에 출력된다.

session leader => login shell process (bash 등)
sessionID = session leader의 PID

#include<unistd.h>

    pid_t getsid(pid_t pid); //프로세스의 SID를 읽어온다.
    pid_t setsid(void); //setsid를 호출한 process가 현재 session의 leader가 아니면 새로운 세션을 만들고 세션리더가 된다.


<Process running time>
: system running time(kernel code, system call) + user running time(user code)

#include<sys/times.h>: man -s 2 times

    clock_t times(struct tms* buf);

리턴: 특정시점으로부터 경과한 시간(clock tick 단위)

struct tms{
    clock_t tms_utime;  //user time
    clock_t tms_stime;  //system time
    clock_t tms_cutime; //user time of children
    clock_t tms_cstime; //system time of children
};


<Environment variables>: 환경 변수
process 실행환경(쉘..)에 정의되어있는 변수. 내가 직접 만들 수도 있다. 
환경변수명은 관례적으로 대문자

<shell commands>
env : 환경변수 목록 확인
export [환경변수명]=[값]

내가 로그인할 때마다 자동으로 환경변수가 설정됐으면 좋겠다?
>> /home(~)으로 가서 ls -a로 .bashrc를 찾아 들어간다. 그 파일 마지막에 export [환경변수명]=[값] 을 적어놓는다.


<getenv(3)>
#include<stdlib.h>

    char* getenv(const char* name); //name에 대응하는 환경변수의 값을 리턴한다. 혹은 NULL 리턴

#include<stdlib.h>

    int putenv(char* string);//"NAME=VALUE"형식의 스트링을 통째로 넣는다.

특징: 아주 위험하고 독특한 녀석입니다. 인자로 준 string의 주소값을 그대로 환경 변수 목록에 등록합니다.
주의사항: 만약 지역 변수(배열)를 putenv에 넣고 함수가 끝나버리면, 환경 변수가 엉뚱한 곳을 가리키게 되어 프로그램이 터질 수 있습니다. 
반드시 전역 변수나 malloc으로 할당한 메모리를 넣어야 안전합니다.

    int setenv(const char* name, const char* value, int overwrite);

동작: name과 value를 받아서 내부적으로 새로운 메모리를 할당하고 내용을 복사해서 저장합니다.(사용자가 관리할 필요가 없어 훨씬 안전)
overwrite:
0: 이미 해당 이름의 환경 변수가 있다면 무시합니다.
1: 이미 있다면 새로운 값으로 덮어씁니다.

    int unsetenv(const char* name);

사용자설정한 환경변수를 삭제함

>>프로세스에서 추가된 환경변수는 자신과 자식 프로세스에게만 유효하다.
>>부모프로세스(=쉘)에겐 영향없다. 
>>또 예를들어 프로세스에서 기존 시스템 환경변수를 putenv등으로 변경했다면프로세스 실행 중에만 바뀌고 나중에 다시 원상복귀된다.


<Running a new process>
1.executing new program: 기존 process는 중지됨. binary program을 읽어와서 자신을 호출한 process의 메모리 영역을 덮어씀.
2.creating new program(fork): 자신을 호출한 process를 복사하여 새로운 process생성. 마치 포크와같이 갈라지는 느낌.

#include<unistd.h>: man -s 3

    int execl(const char *path, const char *arg, .../*(char *)NULL*/); //list
    int execlp(const char *file, const char *rg, .../*(char *)NULL*/); //list, path(env)
    int execle(const char *path, const char *arg, .../*, (char *)NULL, char *const envp[]*/); //list environ

    int execv(const char *path, char *const argv[]); //vector
    int execvp(const char *file, char *const argv[]); //vector, path(env)
    int execvpe(const char *file, char *const argv[], char *const envp[]); //vector, path(env), environ

path/file: 실행할 program binary, file일 경우 현재 디렉토리 및 환경변수 PATH에서 정의된 경로에서 파일을 찾음
arg/argv[]: 프로그램의 인자, 끝에는 반드시 (char *)NULL을 넣어야함
>>argv[0]은 그 프로그램의 이름이다!
envp[]: 프로그램 실행 시 새로 지정할 환경변수, 끝에는 (char *)NULL을 넣어야함

>>리턴 없음, 에러일 경우 -1;
>>리턴이 없는 이유: 성공하면 이미 내 코드가 세상에 없는데, 리턴값을 받아줄 주체(나)가 사라졌기 때문입니다. 그래서 성공하면 리턴하지 않고, 실패할 때만 -1을 리턴하며 원래 코드로 돌아옵니다.

<wait a minute!>
const가 헷갈려요~ >> 이경우에는 *를 오른쪽으로 붙이는게 헷갈리지 않음. C언어에서는 주로 이게 관례
1. 골든 룰: "왼쪽을 얼림, 없으면 오른쪽을 얼림"

A. const char *path vs char *const path
    const char *path: const 왼쪽에 아무것도 없으니 오른쪽의 char를 얼립니다.
의미: *path(내용물)가 const입니다. 즉, "글자 내용은 못 바꾸지만, 포인터가 다른 주소를 가리키게 하는 건 괜찮아!"
*path = 'A'; (X - 에러) / path = next_ptr; (O - 가능)
>>그래서 const char *arr에서 arr++이런걸로 순회 가능하잔슴

    char *const path: const 왼쪽에 *(포인터)가 있습니다. 포인터를 얼립니다.
의미: path(주소값) 자체가 const입니다. 즉, "한번 가리킨 주소는 절대 못 바꾸지만, 그 주소에 들어있는 글자를 수정하는 건 괜찮아!"
*path = 'A'; (O - 가능) / path = next_ptr; (X - 에러)

2. 배열 버전: const char *argv[] vs char *const argv[]
이건 "배열의 각 칸에 무엇이 들어있는가"를 생각하면 쉽습니다.

A. const char *argv[] (가장 많이 쓰는 형태)
해석: (const char *)들이 모인 배열입니다.
의미: "각 칸에 담긴 주소들을 따라갔을 때 나오는 문자열 내용을 수정하지 마라."


B. char *const argv[]
해석: (char *const)들이 모인 배열입니다.
의미: "배열의 각 칸에 저장된 주소값 자체를 바꾸지 마라."
즉, argv[0]에 담긴 주소를 다른 주소로 갈아치울 수 없다는 뜻입니다.


<fork>:man -s 2 fork
#include<unistd.h>

    int fork();
>>포크 진행 과정
1. 부모프로세스에서 fork()호출
2. fork모듈: 새 프로세스 생성 >> 부모프로세스의 메모리 복사
3. fork모듈: 부모에게는 자식의 pid, 자식에게는 0을 리턴
4. 각각 fork리턴 시점 이후로 독립적으로 실행됨

<fork에서 child process의 정보>
상속되는 정보:
-RUID, EUID, RGID, EGID, setuid, setgid
-환경변수, opened file discriptors
-working directory, signal handling setup, resource limits ... etc.
>>즉 메모리상에 있는 정보는 거의다 복사된다고 보면된다.

child proc. 고유정보
-PID: 새로 부여
-PPID(parent PID)
-tms structure
-자원 사용 정보들


<Terminating a process>
프로세스가 종료되면 어떻게 종료되었는지를 exit status에 저장.
>>보통 0은 정상종료, child의 종료상태를 parent에게 전달가능.

#include<stdlib.h>

    void exit(int status);
>>status & 0377(8진수)가 부모에게 전달됨. 리눅스에서는 자식의 종료상태를 1바이트(0~255)범위안에서 전달한다.

<process 종료과정>
1. atexit(3)에 등록된 함수들을 역순으로 수행
2. standard IO stream의 모든 내용을 flush
3. 모든 temporal file들 삭제(tmpfile()..)
4. _exit(2) 호출: 이후 종료과정은 커널이 수행
-사용중이던 fd들 닫기
-부모에게 exit status 전달
-부모에게 SIGCHILD 전달
-자식에게 SIGHUP 전달
-프로세스가 쓰던 자원들 반납

<atexit>: man -s 2 fork
#include<stdlib.h>

    int atexit(void (*function)(void));
>>exit()나 return to main() 등으로 프로세스가 종료될 때 실행할 함수를 등록
>>성공시 0, 실패시 non-zero 값


<zombie process>
종료되었으나 완전히 삭제되지는 않은 프로세스
부모프로세스보다 자식프로세스가 먼저 종료되면 걔는 zombie state가 된다.
>>자원 등은 모두 반납. 하지만 커널에 최소한의 정보가 남아있음(ex. exit status)
부모가 자식의 exit status를 받아들이기 전까지 자식은 zombie process가 된다.

<wait>: man -s 2 wait
#include<sys/types.h>
#include<sys/wait.h>

    pid_t wait(int *stauts); //자식 프로세스가 종료할 때까지 대기
>>child process가 여러개인 경우 그중 가장 빨리 종료된것의 exit status를 얻어옴.
>>실행중인 child process 없는 경우 return -1

<status 값 읽기>: man 페이지를 읽고 활용하는게 중요
상위8비트: exit()으로 전달한 값, 비정상종료시 0(죽기전에 남긴 말)
하위8비트: process를 종료시킨 signal #, 정상종료시 0(어떻게 죽었는지 >> 좀더 급박한 정보)
>>시스템마다 다를수 있음, 이때 매크로함수를 이용할 수 있음

매크로 펑션 이용하기:
1. 정상종료 확인normal exit
WIFEXITED(status):"자식이 exit()이나 return으로 제발로 나갔니?" (true/false)
WEXITSTATUS(status): "그렇다면 그 자식이 남긴 출구 번호(exit status)가 뭐야?" (위에서 배운 하위 8비트 값 추출)
>>왜 하위? 정상종료면 signal # = 0이고, 위 매크로f는 8번의 오른쪽 shift를 수행해 status의 진짜 값을 읽어오는 것이다.

2. 비정상종료 확인 signal exit
WIFSIGNALED(status): "자식이 시그널(예: Ctrl+C, Kill)을 맞고 강제로 끌려 나갔니?"
WTERMSIG(status): "그렇다면 자식을 죽인 시그널 번호가 몇 번이니?"

<waitpid>:man -s 2 wait
#include<sys/types.h>
#include<sys/wait.h>

    pid_t waitpid(pid_t pid, int *status, int options);

pid: 대상 프로세스의 pid
<-1: PIGD가 -pid인 임의의 프로세스
=-1: child process중 하나(임의)
= 0: child process중 자신과 같은 PGID를 가지고 있는 프로세스 중 하나
> 0: PID = pid인 child process

options
WNOHANG: 자식이 종료하기 전이라도, 상태값을 바로 반환. 대기하지 않고 수행계속함. 자식이 아직 안끝났으면 안기다리고 즉시 0 리턴
WNOWAIT: 상태값들 반환한 child process를 대기상태로 유지(좀비 유지), 다시 exit status 요청할 수 있음.

waitpid리턴값
> 0: 종료된 자식의 PID입니다. (성공!)
== 0: WNOHANG 옵션을 썼는데, 아직 종료된 자식이 없을 때입니다. (자식은 살아있음)
== -1: 에러입니다. (기다릴 자식이 없거나 PID가 틀렸을 때)