<disk address>
-물리적 주소는 디스크의 cylinder, surface, secter 각 구간의 숫자 값을 통해 표현가능하다.
-하지만 OS에서는 주소를 쭉 늘어진 일련의 block들로 생각하고 접근한다.
-이를 물리적 주소로 바꿔 디스크와 상효작용이 가능하게 하는 모듈이 disk driver이다.

<block>
-운영체제 입장에서 파일 시스템은 block들의 나열이다.
-각 블록은 512 ~ 8192 바이트의 크기를 가진다.
>> secter size < blcok size < page size(kernel buffer)
블록이란 디스크 접근의 최소단위이다.


<fileIO vs diskIO>
fileIO: 바이트 단위로 접근
diskIO: 블록 단위로 접근
바이트와 블록의 갭이 있으므로 이것이 실시간으로 바이트 단위로 디스크에 적용되진 않는다.
대신 커널에서는 메모리상에서 버퍼를 만들고 여기서 1차적으로 작업을 수행하다가 필요한 때에 디스크에 저장한다.(page write back)

page write back 시점:
1. 커널의 메모리 공간이 부족할 때
2. 일정 시간이 지났을 때
3. fsync(2) 호출을 통한 강제 동기화
>> page write back 시점은 유저가 예측하기 어렵고 비효율적일 수 있다.

이에 대한 해결방안: 유저가 직접 버퍼의 크기를 정의하고(char buf[bock_size]) write하는 타이밍도 정해주자.
그런데 이렇게 직접 하기 귀찮으므로 탄생한 것이 stdio.h이다. 이걸 사용자 버퍼 IO라 부른다.


<standard io>
platform-independent, user buffering solution

파일 포인터: 파일 오퍼레이션을 관리하는 FILE구조체의 포인터. FILE *fp;
이것이 가리키는 곳에는 버퍼 주소, 버퍼 내 문자 등의 정보가 있으며, file descriptor의 정보도 함께 저장되어 있다.
즉, high-level IO 이지만, 결국 low-level을 거쳐야한다는 의미이다.
즉, 파일 포인터는 내부적으로 파일 디스크립터와 mapping된다.

stream: file과 program을 이어주는 통로, byte들의 흐름.


<fopen(3)> man -s 3 fopen

FILE* fopen(const char *path, const char *mode);
open과 달리 flag와 mode로 구분하지 않는다. 좀더 직관적이다.
리턴 값은 파일 포인터이며, 실패시 NULL을 리턴한다.

mode 종류:
r: 파일이 존재하지 않으면 열기 실패한다.
w: 파일이 존재하지 않으면 생성한다. 덮어쓴다.
a: 파일이 존재하지 않으면 생성한다. 이어쓴다.

r+: 읽기모드, 추후 쓰기모드로 전환가능
w+: 쓰기모드, 추후 읽기모드로 전환가능
a+: 덧붙이기, 추후 읽기모드로 전환가능
>>여기서 주의점: 읽기/쓰기 모드 전환시 반드시 다음의 함수 중 하나를 호출해야 함.
fflush(), fseek(), fstepos(), rewind()

b: 이진 파일 모드. 읽기/쓰기 모드는 위에 나온 표현 뒤에 b를 붙인다. 예를들어 rb, wb 등.
binary file은 text file에 비해 효율이 좋다. 비록 우리가 이해할 수 있는 형태는 아니지만.

<fclose(3)>
int fclose(FILE *stream);
성공하면 0, 에러는 -1 리턴


file access categories
-character IO
-string IO
-binary IO
-formatted IO


<character IO>
int fgetc(FILE *stream);
int getc(FILE *stream); //매크로로 정의되어있음. 따라서 코드에 바로 적용되어 함수 오버헤드가 발생하지않는다.
//다만, 매크로이기 때문에 인자에 수식을 넣는다던지 하는 일은 불가능하다는 걸 참고.
int getchar(void); //키보드로부터 인풋을 받음. getchar(void)는 getc(stdin)과 동일하다.

int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c); // putchar(c) = putc(c, stdout)이다.
>>int c인 이유: EOF가 -1이어서 이것 까지 포함하기 위해서

읽은/기록한 문자를 리턴한다. 아스키 코드도 숫자이기 때문이다.
EOF이거나 에러라면 -1을 리턴한다.


<string IO>
char* gets(char* s); //stdin에서 스트링을 s에 입력받는다. 버퍼 크기에 대한 고려가 없어 보안문제가 있다.
//비슷하게 character IO의 함수들도 반복문을 통해 사용할 경우 배열의 크기를 넘어가지 않는 지 고려하여 보안 문제를 예방해야한다.
//따라서 gets말고 fgets를 써야한다.

char* fgets(char* s, int n, FILE* stream);
n: 버퍼의 크기, stream에서 최대 n-1 개의 문자를 읽어와서 저장한다. \n 또는 EOF를 만나면 거기 까지만 읽어온다.
return: 버퍼의 시작주소, 읽을게 없으면 NULL을 리턴


int puts(char* s); //put to stdout (with \n)
int fputs(char* s, FILE* stream); //s를 stream에 출력. 따로 \n을 추가하진 않음.
>> 성공은 양수, 실패는 0을 리턴


stringIO.c 참고사항: vi에디터는 txt등 파일을 적고 저장하고 나갈 때 자기가 알아서 \n를 추가한다. 그래서 fgets, fputs를 사용했는데
어디서 \n가 나왔나 했더니 범인이 vi였다. 이 해결 방법은 printf "hello!" > hello.txt 등 쉘커멘드로 입력해서 \n을 포함안시킬 수 있다.


<binary IO>
size_t fread(void* ptr, size_t size, size_t memb, FILE* stream);
//stream에서 크기가 size인 아이템을 memb수 만큼 읽어와서 ptr에 저장하겠다.

size_t fwrite(const void* ptr, size_t size, size_t memb, FILE* stream);
//ptr에서 크기가 size인 아이템을 memb수 만큼 읽어와서 stream에 저장하겠다.

return: read/write 한 item의 수 / 혹은 EOF를 리턴


cf. binary 파일의 내용을 16진수로 보고싶다면 xxd hello.bin 이런 명령어를 사용하면 된다.


cf. fprintf(stderr, "Fail to open the file - %s\n", filename);
나중에 어떤 프로그램의 결과를 한곳에 모아서 본다고 가정해보자.
이때 fprintf로 stream을 stderr로 지정해줌으로써 일반적인 결괏값만 보고, 나머지 에러값은 그때그때 콘솔화면에
나타나게끔 할 수 있다. perror()도 마찬가지 맥락이다. 결국은 stderr를 활용한다는게 중요하다.


