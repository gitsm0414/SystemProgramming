<files in unix/linux>
cd / : root로 간다. 최상위 폴더
cd ~ : home으로 간다. 

파일의 종류
regular file (-)
special files
-character device file (c) 
-block device file (b)
directory (d)
symbolic link file (l): 이미 존재하는 파일이나 디렉토리에 접근할 수 있는 새로운 이름
socket file (s)
name pipe (p)
...

우리가 ls -l을 했을 때 앞에 나타나는 정보에서 rwx가 나타나는 곳이 있는데
그 중 맨 앞의 칸이 파일의 종류를 의미한다. 여기서 괄호 안의 문자들이 쓰인다.


<special file>: 장치와 데이터를 주고받는 '통로', 실제 디스크에 공간을 차지하고 있지 않다.
즉 데이터 블록이 없다.
디바이스 번호를 inode에 저장.
(c), (b)는 각각 데이터 전송의 단위가 character, block이다.

cf. 윈도우에서는 최상위폴더가 \인데, 리눅스에서는 \는 escape문자취급해서 루트가고싶으면 /를 써야한다.

ls -l /dev를 치면 예를들어
...
drwxrwxrwt 2 root root          40 Mar 16 11:38 mqueue
drwxr-xr-x 2 root root          60 Mar 16 11:38 net
crw-rw-rw- 1 root root      1,   3 Mar 16 11:38 null
crw------- 1 root root     10, 144 Mar 16 11:38 nvram
...
이렇게 나오는데, 여기서 special file들은 중간에 1, 3, 10, 144같이 숫자를 가지고있다.
첫번째는 major device number: 어떤 드라이버를 쓸지
두번째는 minor device number: 해당 드라이버가 관리하는 디바이스 중 몇번째인지

그리고 device란 용어를 썼다고 해서 그게 꼭 물리적인 장치만을 의미하지는 않는다.
software device도 존재한다. 광범위적인 용어인 것이다.


<file organization>
파일은 크게 세 부분으로 구성된다.
1. 파일명(hard link)
2. inode: 파일의 정보를 담고있다. 번호를 통해 관리/접근한다.
ls -i를 통해 inode번호를 확인한다.
3. data block: 실제 데이터가 저장된 디스크 공간

<inode의 구성>
-inode 번호

-파일에 대한 여러가지 정보들 ...

-데이터블록을 가리키는 주소

<파일 접근 과정>
file open 시...
[fd array] -> [file table] -> [inode] -> [disk]
-fd array의 실제 값은 file table을 가리키는 포인터이다.
-프로세스마다 fd array를 각 하나씩 가지고있다. 따라서 fd값은 프로세스끼리 중복 가능하다.
-하지만 file table은 각각이 시스템 전체에 하나만 존재한다. 같은 inode를 가리키더라도 서로 다른 file table을 가질 수도 있는 것이다.
-file table은 file offset, 파일 권한 등 정보를 가지며, inode번호또한 가지고 있어서 inode로 연결 가능하다.

<stat(2)>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

    int stat(const char* pathname, struct stat* buf);
    int fstat(int fd, struct stat* buf);

inode에 저장된 파일 정보를 읽어옴. 성공시 0, 실패시 -1



//stat 구조체는 파일 테이블에 있는 것도, inode 자체도 아님. **"inode중 도움 될 정보를 읽어와 사용자에게 보여주기 위한 임시 바구니"**
<stat structure>: 참고용, 더 보고싶으면 man 2 stat
struct stat {
    dev_t   st_dev;
    ino_t   st_ino; //inode number
    mode_t  st_mode; //protection
    nlink_t st_nlink;
    uid_t   st_uid;
    gid_t   st_gid;
    dev_t   st_rdev; //device file이라면 이걸 통해 major, minor number를 알수있다.
    off_t   st_size; //total size, in bytes
    blksize_t   st_blksize; //blocksize for filesystem I/O
    blkcnt_t    st_blocks; //number of 512B blocks allocated
    struct timespec st_atim; //last access time
    struct timespec st_mtim; //last modification time
    struct timespec st_ctim; //last status change time
};


printf("Mode = %o\n", (unsigned int)buf.st_mode); //mode값은 8진수로 표현하는것이 관례
이값을 실행해보면 Mode = 100644 라고 나올탠데 이는
10: regular file을 의미
0: 특수 권한 없음
644: 사용자는 r,w(6) 나머지는 r(4)권한

st_mode는 총 16비트인데 이는 각각
4bit    3bit    3bit    3bit    3bit
파일종류 특수권한  사용자   그룹    others


<file type checking>
st_mode의 최상위 4bit확인
<sys/stat.h>에 정의된 symbolic constants 사용
상수명      16진수      기능
S_IFMT     0xF000     st_mode 값에서 파일 종류를 정의한 부분만 따로 가져옴
S_IFIFO               FIFO 파일
S_IFCHR               character device file
S_IFDIR               ...
S_IFBLK
S_IFREG
S_IFLNK             
S_IFSOCK              ...


상수로 st_mode & S_IFMT 이렇게 구하고 케이스별로 나누는 게 귀찮으면 매크로 함수를 쓸 수 있다.
macro function: 실무에서는 위의 상수보다는 매크로함수를 이용하는게 좋다. 왜냐하면 시스템마다 파일 종류를 나타내는 비트위치가 달라질 수 있는데
매크로함수를 쓰면 커널이 알아서 그에맞게 적용해주기 때문이다.(이식성이 좋다.)

S_ISFIFO(mode) : (((mode) & 0xF000) == 0x1000) ? 참이면 FIFO파일
S_ISCHR(mode)
...


이 외에도 쓸수있는 symbolic constat는...
S_ISUID
S_ISGID
s-ISVTX
>>특수권한 쪽. & 연산에 활용할 수있다.
S_IREAD
S_IWRITE
S_IEXEC
>>& 연산으로 소유자의 권한을 확인할 수 있다.
man page에 다 나와있으니 이런게 있다는것만 알아두자.



<access(2)>: 현재 process(내 프로그램)이 파일에 대해 가진 권한 확인
#include<unistd.h>

    int access(const char* pathname, int mode);

mode: 확인할 권한
R_OK
W_OK
X_OK
F_OK(파일 존재유무 확인)

0: 권한 있음. 
-1: 권한 없음. (errno = ENOENT 파일 없을 때) (errno = EACCES 접근권한 없을 때)



<chmod(2)>
#include<unistd.h>

    int chmod(const char* filename, int mode);
    int fchmod(int fd, int mode);

chmod를 시스템콜로도 이용가능하다.
모드는 아래의 상수들을 이용가능하고 | 가능하다.

S_IRWXU
S_IRUSR
S_IWUSR
S_IXUSR
...


<directory>
파일 이름 - inode 번호의 매칭들을 담고있는 파일.
-subdirectory: 디렉토리는 디렉토리를 담을 수 있다.
-절대경로: /에서 시작하는 경로, 상대경로: .에서 시작하는 경로

<기본적인 명령어들>: man -s 2 ...
>> 실패시 -1, 성공시 0 리턴
#include<sys/types.h>
#include<sys/stat.h>

    int mkdir(const char* pathname, mode_t mode);

#include<unistd.h>

    int rmdir(const char* pathname);

#include<stdio.h>

    int rename(const char* oldpath, const char* newpath); //mv랑 똑같다. 따라서 파일/디렉토리의 이름 뿐만아니라 위치도 변경가능하다.
    
예를들어 rename("study.txt", "my_study/study.txt");

#include<unistd.h>

    char* getcwd(char* buf, size_t size); //get current working directory

>>buf가 NULL일 경우 내부에서 동적할당 후 주소리턴

#include<unistd.h>

    int chdir(const char* path); //쉘의 cd와 같다.

>>working directory를 바꾼다. 실패시 -1 리턴, 성공시 0



<open/close directory>: man -s 3 ...
#include<sys/types.h>
#include<dirent.h> //중요

    DIR* opendir(const char* name);
name: 열려는 디렉토리 이름

    int closedir(DIR* dirp);
dirp: 닫으려는 DIR포인터
리턴: -1 or 0


#include<dirent.h>

    struct dirent* readdir(DIR* dirp);

디렉토리의 내용을 하나씩 읽어옴. ->..이랑 .이랑 subdirectory나 하위file등이 각각의 엔트리가 된다.

cf.
struct dirent{
    ino_t   d_ino;
    off_t   d_off; //다음 dir entry를 찾기위한 불연속적인 '쿠키값'
    unsigned short  d_reclen; //dirent 구조체 자체의 크기. d_name에 따라 달라지기 때문
    unsigned char   d_type; //type of file 디렉토리? 레귤러파일? 심볼릭링크?
    char    d_name[256]; //file name 보통 최대 256
};


<directory offset>: man -s 3 ...
디렉토리는 오프셋이 움직이는 단위가 dirent하나이다.(바이트 단위인 일반 파일과 달리)

#include<dirent.h>

    long telldir(DIR* dirp); //현제 오프셋의 위치를 알려준다.

    void seekdir(DIR* dirp, long loc); 
    //오프셋의 위치를 변경한다. 오프셋이 loc인 위치로 옮긴다.
    //참고로 loc에 0, 1, 2같은 숫자를 넣으면 의도한대로 동작하지 않는다. 왜냐하면 리눅스파일시스템에서는
    //오프셋 값이 크고 불연속적이기 때문이다.(단순 리스트가 아니라 해쉬트리로 관리한다고 한다.)
    //그래서 보통 readdir로 몇번 읽기만해서 특정 위치로 이동한 뒤 작업하거나, 읽어들이는 과정에서 다시 돌아오고싶은 오프셋을 변수로 따로 저장하기도 한다.

    void rewinddir(DIR* dirp); //0번 오프셋으로

<데이터 흐름지도 참고>
cf. FILE*, DIR*같은 구조체는 file table을 가리키는 포인터가 아니라 내부적으로 fd를 품고있는 '사용자 정의 구조체'이다. 훨씬 고수준이다.
(저수준 시스템콜에 비해)그 안에는 fd정보, 버퍼, 어디까지 읽었는지 등의 정보가 저장되어있다. 

[사용자 프로그램] >   ↓ (FILE* 또는 DIR* 포인터 참조)
[표준 라이브러리 구조체 (내부에 fd 보관)]
↓ (구조체 안의 fd 값을 꺼내서 시스템 콜 호출)
[커널: FD Array] (프로세스별)
↓
[커널: File Table] (시스템 전체)
↓
[커널: Inode / Disk]


<hard link>
디렉토리안에 있는 파일-inode번호간의 매핑을 하드링크라 볼 수 있다.
-하드링크는 여러개 생성할 수 있다.
ex) 파일은 다르지만 동일한 아이노드를 매핑할 수 있다.(즉 inode는 하나)
>>이런게 inode structure의 멤버 중 '링크 수'를 의미한다.

-하지만 다른 file system에 있는 inode에 대한 hard link는 불가능하다.

<symbolic link(soft link)>
실제 파일의 절대경로명을 저장하는 파일이 있다치자.
이 파일에 접근하여 1차적으로 가고싶은 파일의 경로명을 알아내고, 이것으로부터 실제 파일에 접근하는 것이다.
두 단계를 거친다. 그리고 각 파일의 inode는 서로 다르다!
-다른 file system의 파일/폴더를 연결할 수 있다.

여기서 ls -il을 해보면 inode도 서로 다르고, rwx..의 맨 앞칸이 -(일반파일)인 hard link와 달리
맨 앞칸이 l(link)로 파일종류도 다름을 알수있다.


<shell command for link>
ln [target] [linkpath]
-d directory에 대한 링크생성(root권한 필요)
-s soft link / 기본옵션은 hard link이다.

cf. ls -il 에서 i는 inode값을 출력해준다.

-소프트링크는 예를들어 윈도우와의 공유폴더를 링크할때도 사용할 수 있다.
ex) ln -s /mnt/d/LinuxShare/ sharedFolder
>>리눅스상에서 sharedFolder에 접근하면 윈도우시스템과도 파일을 공유할 수 있다.


<link systemcalls>: man -s 2 ...
#include<unistd.h>

    int link(const char* oldpath, const char* newpath); //hard link 생성

    int symlink(const char* target, const char* linkpath); //soft link 생성

실패시 -1 리턴


<링크 삭제>
#inlcude<unistd.h>
    int unlink(const char* pathname); //man -s 2 unlink

#include<stdio.h>
    int remove(const char* pathname); //man -s 3 remove

>>pathname: 삭제하려는 hard link의 경로(=파일이름)

unlink(): 파일시스템에서 이름을 삭제. 링크가 하나만 남았다면 사실상 파일삭제. 디렉토리에는 적용불가
remove(): unlink와 동일한 기능, 디렉토리에도 사용가능.(단 빈 디렉토리일 경우만)

//성공은 0, 실패는 -1


#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

    int lstat(const char* pathname, struct stat* buf);

>>symbolic link 파일 자체의 정보를 가져온다. 만약 stat()을 호출했다면, 심볼릭링크를 거쳐 그것이 가리키는 최종목적파일의
>>정보를 읽어올 것이다. 그래서 lstat이란 함수가 따로 있다. 
//성공은 0, 실패는 -1



<링크 정보>
#inlcude<unistd.h>
: man -s 2 readlink

    ssize_t readlink(const char* pathname, char* buf, size_t bufsiz); //결과 읽은 byte 수 혹은 -1
>>symbolic link파일에 저장된 값을 buf에 읽어온다. 즉 최종목적파일의 상대경로가 읽힐것이다.
>>마지막에 '\0'추가 안해주므로 직접 넣어줘야한다.

#include<stdlib.h>
#include<limits.h>
: man -s 3 realpath

    char* realpath(const char* pathname, char* resolved_path);

>>심볼릭링크가 가리키는 파일의 절대경로를 얻을 수 있다.
resolved_path 가 NULL이면 내부적으로 동적할당 후 포인터 반환

리턴: 결과가 저장된 buffer의 포인터, 실패시 NULL