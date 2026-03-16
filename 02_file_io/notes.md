리눅스에서 파일의 종류
1. regular file(text, binary)
2. directory
3. special file
-파일 형태로 표현된 커널 내 객체
-device files, pipes, socket, etc.

Everything is a file!


ls(list)
-l(long) 상세히 출력
-a(all) 숨겨진 파일도 출력
>>파일이름 앞에 .이 있으면 숨겨진 파일이란 뜻

둘이 합치면 -al


touch: 빈 파일 생성 혹은 변경된날짜(time stamp) 변경


<file access permission>p
-owner, group, others 사용자에 따라 권한을 가질 수 있음
r, w, x(execute) 순서대로 한비트씩 할당되는데 위 순서대로 총 9비트가 파일권한을 나타낸다.
owner group others
r w x r w x r w x

chmod: 파일권한 변경
>> chmod 755 <filename> 이때 숫자는 8진수 세자리이다. 위의 권한 표를 참고.
>> chmod u+r <filename> user에게 read권한을 추가하겠다는 뜻


<low-level file IO>
-system call 이용
-file descriptor 사용
-바이트 단위 출력
-특수파일을 다룰 수 있음

<high level file IO>
-C standard library
-File pointer사용 (FILE *fp)
-버퍼(block)단위 입출력

<open(2)>
#include<sys/types.h> //시스템 데이터타입의 정의. mode_t(파일권한설정 시), off_t(파일 오프셋), pid_t(프로세스 아이디) 등..
#include<sys/stat.h> //파일의 상태와 권한설정 (status) 읽기/쓰기/실행 권한설정
#include<fcntl.h> //파일 제어 플래그(file control). 파일을 어떤방식으로 열것인가에 대한 상수를 제공

int open(const char *pathname, int flags [, mode_t mode])
-pathname: 파일경로, 열려는 파일 포함
-flags: access mode 설정
-mode: 권한설정. 파일을 새로 생성할 때만 유효
마지막으로 file descriptor를 리턴한다.


<file descriptor>
-열려있는 파일을 표현하는 정수 값(int), 이를 통해 파일에 접근한다.
-파일을 열때마다 순차적으로 할당. pocess당 최대 할당 수 1,024
Default fds
-0: stdin
-1: stdout
-2: stderr

<flags>: | bit-OR 사용해 조합
O_RDONLY
O_WRONLY
O_RDWR
O_CREAT: 파일이 없으면 파일을 생성한다.
O_EXCL: O_CREAT과 함께 사용할 경우, 파일이 없으면 파일을 생성하지만, 이미 파일이 있다면 에러메시지를 출력한다.
O_APPEND: 파일의 맨 끝에 내용추가
O_TRUNC: 이미 있는 파일이고, 쓰기 옵션으로 열었으면, 모든 내용을 지우고 파일의 길이를 0으로 변경한다.
O_SYNC/O_DSYNC: 파일데이터 및 메타데이터(파일의 모든 정보)가 하드디스크에 저장될 때까지 기다림(속도느림)(누락있으면 안되는DB분야에서 중요)/파일데이터만 기다림(실속형)
>>커널 버퍼에만 저장한 뒤 저장완료라고 보고하는데 보통, 이 경우 전원이 꺼지면 데이터가 증발할 수 있음.(HDD까지 못갔으니) 이를 방지하기 위함.


<file table>: 커널이 process별로 유지, 열린 파일에 대한 각종 정보 관리

<mode>: 파일의 권한값을 설정 |로 조합가능 
S_IRWXU
S_IRUSR
S_IWUSR
S_IXUSR

>>G, GRP, O, OTH에 대해서도 동일한 패턴.


<file close>
#include<unistd.h> //파일을 읽고, 쓰고, 닫는 등의 동작에 대한 것들이 담겨있다. unix standard의 줄임.

int close(int fd);
0: 정상작동, -1: error

<Error handlinig for system call>
system call은 에러시 -1을 반환
에러코드는 errno에 저장됨(error.h에 있음)
extern int errno; 로 직접 접근가능
>하지만 요즘은 #include <errno.h>하면 내부적으로 extern처리가 되긴한다. 혹은 perror()를 사용한다.


<read, write(2)>
ssize_t read(int fd, void *buf, size_t count);
-buf의 경우 함수의 인자로 보내기 전에 (void*)로 형변환을 하는 습관을 들이면 좋다.
-count: 읽어들일 바이트 수
-리턴값: 읽은 바이트 수 혹은 EOF에 도달하면 0, 에러는 -1

ssize_t write(int fd, const void *buf, size_t count);
실제로 쓴 바이트 수를 리턴한다.

<File offset>: file position, 시작점부터 현재까지의 바이트 수


cf. 간단하게 파일 내용을 만들 때 가능한 방법
$ehco "~~~~~~" > hello.txt


<lseek(2)>
off_t lseek(int fd, off_t offset, int whence);

>>whence로부터 offset만큼 오프셋을 이동한다.(양,음수 가능)
이동 후 오프셋의 값을 리턴한다.(에러-1)

whence
-SEEK_SET
-SEEK_END
-SEEK_CUR


SW와 DISK사이의 I/O 작업은 cost가 크기 때문에, 그 사이에 page cache를 이용해 작업 속도를 높인다.
마찬가지로 page write-back도 작업속도를 높일 수 있다. DISK에 반영하는 시기는 kernel이 결정한다.
>>
#include<unistd.h>

int fsync(int fd);

page write-back을 강제로 수행. 성공 0, 에러 -1


<fcntl(2)>: manipulating fd/ 자세한 내용은 man -s 2 fcntl 참고

int fcntl(int fd, int cmd, /*args*/);
cmd: 수행할 명령
-F_GETFL (get flags), F_SETFL (set flags) 외에 도 많은 옵션들이 존재
args: cmd에 따라 필요한 인자들

리턴: cmd에 따라 다름.

<dup(2)> man -s 2 dup
#include<unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);

oldfd가 가리리키는 파일을 똑같이 가리키는 새로운 파일디스크립터를 할당한다. 리턴 값은 새로운 fd의 값이다.
dup의 경우 가능한 fd값 중 가장 작은 값으로, dup2는 newfd의 값으로 지정된다.
>> ioredirection.c 코드 참고



