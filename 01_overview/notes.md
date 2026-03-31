시스템 프로그래밍이란?
-소프트웨어 및 유저를 위한 서비스를 제공하는 것, 그를 위한 소프트웨어를 만드는 것
-시스템 호출을 이용해서 프로그램을 작성하는 것. 시스템호출은 시스템이 제공하는 서비스를 프로그램에서 쓸 수 있도록 해준다.
>우리의 목적: 시스템에 대한 이해를 바탕으로 프로그램을 좀 더 효율적으로 짤 수 있다.


넓은 맥락에서의 시스템: 하나의 목적을 달성하기 위해서 HW or SW 자원들을 모아서 묶어놓은 것.

system call: 운영체제에게 어떠한 기능을 요청하는 것, 그 통로를 system call interface(ex. 함수)라 부른다.
system SW(a.k.a. utilities): 응용 프로그램을 만들려고 system call을 이용할 때 도움을 주기 위한 SW


system call > 커널 모듈을 직접 실행 / 반면 library function > 함수 내부적으로 시스템 콜을 실행


Why system programming?
-더 많은 것을 할 수 있다. 중간 다리를 거치는 것 보다 직접 시스템에게 요구하는 게 더 구체적이고 제약이 덜하다.
-퍼포먼스를 올릴 수 있다. 프로그램의 성능/효울을 높일 수 있다.


unix/linux system: 현재 다양한 시스템에서 가장 널리 사용되는 OS (ex. android, ios, Mac OS, ... )


kernel: OS의 핵심 기능을 모아논 것
system call: OS의 기능 중 응용프로그램이 사용하도록 허락된 기능들의 집합
shell: 유저와 OS사이의 인터페이스. 사용자의 명령을 해석하여 커널의 함수를 호출한다.

>>커널(Kernel)이란 도대체 무엇인가?
자꾸 나오는 '커널'이라는 녀석, 어렵게 생각하실 필요 없습니다. 커널은 "하드웨어를 관리하는 거대한 비서 실장" 프로그램입니다.
커널은 하드웨어를 재료료하는 요리사이며, 우리(유저)는 프로그램(주문서)을 통해 커널에게 주문을할 수 있습니다.


<linux>
-pc를 위한 효율적인 unix system
-open source이며, 다양한 플랫폼에 이식가능하다.
-안정성이 좋다.
-강력한 네트워크 기능을 가졌다.


<basic unix/linux commands>

1. 로그인/아웃
telnet: 유닉스 시스템에 접속(원격접속을 할 때..)
logout, exit: 유닉스 시스템 접속해제

2. 프로세스 관리
ps: 현재 실행중인 프로세스의 정보 출력. 
-u <userid>: 특정 사용자의 프로세스
-ef: 모든 사용자의 프로세스

kill <process id>: 프로세스 강제종료
-9: '강제'종료

3. 파일/디렉토리
pwd: 현재 디렉토리 경로

ls: 디렉토리 내용 출력

cd: 현재 디렉토리 변경
cf. 상위디렉토리: .. , 현재 디렉토리: . ,최상단: ~

cp <파일명> <생성할이름>: 파일/디렉토리 복사
-r: 디렉토리 복사

mv <파일/디렉토리> <이동할위치/바뀔이름>: 파일/디렉토리 이름변경과 이동
ex) mv hello.txt systemProgramming/

rm <파일/디렉토리>: 파일/디렉토리 삭제
-r: 디렉토리 삭제
-f: 강제수행. 사용자확인절차를 거치지 않고 바로 삭제한다.

mkdir, rmdir: 빈 디렉토리 생성, 삭제

cat: 파일 내용 출력(concatenate: 연결하다)

more: 파일 내용을 쪽단위로 출력

chmod: 파일 접근권한 변경

grep <패턴/내용> <검색할파일>:파일 내의 패턴(내용)을 검색


4. others
su: 사용자 계정 변경

tar: 파일/디렉토리 묶기
-cvf a.tar *: 현재 폴더의 모든 파일을 압축(tar파일 생성)
-tvf a.tar: tar파일 내용보기
-xvf a.tar: tar파일 풀기

whereis, which: 파일 위치 검색



<Man page>
Shell command(1), system call(2), standard library(3)에 대한 상세설명 제공

$man [options] [section] page
page: 검색하려는 명령어/함수 이름
section: page의 소속구분 1, 2, 3
-s: 섹션을 지정해서 검색하겠다는 옵션이다.


<VI editor>
입력모드전환: i(insert), a(add), o(다음줄), O
명령모드전환: <Esc> 이후 :을 입력하여 명령수행가능
커서이동: h,j,k,l 또는 방향키
행이동: G(맨 마지막 줄) #G(#번째 줄로 이동)또는 :행번호 (:3)

undo: u
redo: <Ctrl> + r

저장: w (:wq 저장하고 종료)
종료: q (:q! 저장안하고 종료)

복사: #yy
붙여넣기: p

글자삭제: x, #x
행삭제(잘라내기): dd, #


<GNU C compiler>

$gcc [options] filename
-c: 오브젝트파일(.o) 생성
-o: 이름 설정( 기본은 a.out )


<Makefile & make>
>>실수 방지 및 수정된 파일만 컴파일에 포함시킴으로써 최적화

Makefile 형식
<target>: <dependencies>
    <recipe>
...

이후에 $make


implicit rules
-(.c)파일을 (.o)파일까지 컴파일하는 규칙은 target: dependencies만 명시해줘도 된다.
-단, 최종 결과물의 형식은 전부 상세히 적는다.


Variablefs(or Macro)
ex)
CC = gcc
CFLAGS = -g -Wall //-g gdb같은 디버깅툴을 위한 밑작업을 함. 대신 파일크기가 커짐. 그래서 보통 개발할때만 사용하는 옵션
OBJS = main.o foo.o bar.o
TARGET = app.out

all: $(TARGET)  #make를 할 때 가장 먼저 보는 녀석. 타겟을 적는것이 일반적인 패턴

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $@ $(OBJS)  # $@는 current target name을 뜻하는 automatic variable이다.

main.o: foo.h bar.h main.c
foo.o: foo.h foo.c
bar.o: bar.h bar.c

clean:  #clean을 수행할 때는 make clean이라 명령한다.
    rm -f *.o
    rm -f $(TARGET)



<Makefile basic pattern>

CC = <compiler>
CFLAGS = <options for compiler>
LDFLAGS = <options for linker>
LDLIBS = <a list of library to link>
OBJS = <a list of object file>
TARGET = <build target name>

all: $(TARGET)

clean:
    rm -f *.o
    rm -f $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $@ $(OBJS)

...이후 하위 타겟에 대해서는 target: dependencies 관계를 적어준다.

cf.
$@: name of the target of the rule
$^: name of all the dependencies of the rule (dependencies = prerequisites)


cf. CMake
-Makefile을 자동으로 생성해주고, 기타 유용한 기능을 제공한다. (예. 라이브러리 탐색)
-주로 오픈소스를 활용할 때 필요하다.