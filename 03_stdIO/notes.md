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

b: 이진 파일 모드. 읽기/쓰기 모드는 위에 나온 표현 뒤에 b를 붙인다. 예를들어 rb, wb+ 등.
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

return: read/write 한 item의 수 (EOF는 -1이므로 fread, fwrite는 EOF를 리턴하지 않는다.)


cf. binary 파일의 내용을 16진수로 보고싶다면 xxd hello.bin 이런 명령어를 사용하면 된다.


cf. fprintf(stderr, "Fail to open the file - %s\n", filename);
나중에 어떤 프로그램의 결과를 한곳에 모아서 본다고 가정해보자.
이때 fprintf로 stream을 stderr로 지정해줌으로써 일반적인 결괏값만 보고, 나머지 에러값은 그때그때 콘솔화면에
나타나게끔 할 수 있다. perror()도 마찬가지 맥락이다. 결국은 stderr를 활용한다는게 중요하다.


<ASKII vs binary>
text file
-사람이 바로 읽을 수 있음
-데이터 저장 및 사용 시 변환과정이 필요함. 따라서 효율 감소
-필요 저장공간도 늘어남. 예를들어 1234.56789는 text file에서 10 bytes, 이진파일에서 4 bytes이다.
>>따라서 많은 양의 데이터 처리에는 비효율적이다.

binary file
-별도의 변환과정이 필요없어, 시간/공간 적으로 효율적이다.
-다만 사람이 읽을 수 없는 형태이므로 데이터 교환 시 약속(protocol)이 필요함.



<formatted IO>
int fscanf(FILE* stream, const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);
리턴: 입출력한 문자 수 | 음수: error


<fflush()>: man -s 3 fflush
int fflush(FILE* stream);
fflush는 유저공간의 버퍼를 비워 커널 공간의 버퍼로 옮기는 작업이다. 이후는 커널에게 달려있기 때문에
디스크에 동기화됐는지는 알 수 없다.
stream이 가리키는 버퍼의 내용을 모두 비운다.
성공시 0을 반환하고, 실패시 EOF를 반환하며 errno가 설정된다.


<setvbuf()>: man -s 3 setvbuf
int setvbuf(FILE* stream, char* buf, int mode, size_t size);
stream: 설정을 변경할 파일포인터, 예: stdout, fp, ...
buf: 사용자가 할당한 버퍼 메모리 주소, NULL일 경우 시스템이 알아서 할당
mode: 버퍼링의 종류
size: 버퍼의 크기

모드:
_IOFBF: full buffering. 버퍼가 꽉차야만 비움. 파일기록 시 성능 최적화에 도움
_IOLBF: line buffering. \n을 만나면 버퍼 비움. 주로 콘솔(터미널)출력 등 사용
_IONBF: no buffering. 버퍼를 쓰지 않음. 즉시 입출력. 주로 stderr등에 쓰임

ex) 성능 최적화를 위한 큰 버퍼 설정
char mybuf[8192];
setvbuf(fp, mybuf, _IOFBF, 8192);

주의사항:
1. fopen으로 열고 난 직후, 그리고 첫번째 입출력함수를 사용하기 전에, 즉 그 사이에 설정을 해줘야함. 안그럼 적용 안될수도.
2. 사용자 지정 버퍼를 지역변수로 만들어 함수에 보내면 고려해야할 것들이 많다. 그래서 웬만하면 NULL을 전달하는 걸 추천.



<Handling file offset(high level)>: lseek()과 헷갈리지 말기

long fseek(FILE* stream, long offset, int whence);
whence로부터 offset만큼 오프셋을 이동시킨다.
whence: SEEK_SET, SEEK_END, SEEK_CUR

long ftell(FILE* stream); // 현재 오프셋의 위치를 알려줘
void rewind(FILE* stream); // 오프셋 되감기, 맨 처음으로


//파일의 크기가 2GB를 넘긴다면 fseek, ftell로 포지션범위를 다 표현하지 못한다.
//아래 함수들은 크고 복잡한 파일을 유연하게 다루기에 유리하다. 포지션을 따로 fpos_t라는 타입에 저장하기 때문이다.

int fgetpos(FILE* stream, fpos_t* pos);
현재 파일포인터의 위치 정보를 pos 변수에 저장한다. 성공 시 0, 아니면 그외 값 및 errno를 설정한다.

int fsetpos(FILE* stream, const fpos_t* pos);
fgetpos로 pos에 저장해둔 값을 바탕으로 파일 포인터를 해당위치로 복구시킨다. 성공 시 0, 아니면 그외 값 및 errno를 설정한다.


<fd ~ fp>
FILE* fdopen(int fd, const char* mode); //file discriptor open

open을 이용해 low-level에서 열었던 파일을 high-level에서 다룰 수 있도록 파일포인터를 생성해 반환해준다.
이때 모드는 fd로 열었을 때의 모드와 동일해야한다.
리턴 값은 fp 또는 NULL이다.

int fileno(FILE* stream); // file number

fp로 열었던 파일의 fd값을 반환해줌으로써 low-level에서 파일을 다룰 수 있게끔 한다.
리턴값은 fd값 혹은 -1이다.


cf. fd -> fdopen을 호출하는 순간 fd는 FILE 구조체(스트림)안에 포섭되게 된다. 따라서 파일을 닫을 때는
close(fd)가 아니라 fclose(fp)를 사용한다. 이러면 자동적으로 파일이 가지고 있던 사용자 버퍼를 비우고
메모리를 해제하며, close(fd)시스템 콜을 자동으로 호출한다.

따라서 온전히 low-level만 존재하는게 아닌 이상은 무조건 fclose를 이용하자.

또한 close()이후 fclose()를 사용해서도 안된다. 그러면 결과적으로 close(fd)가 이미 됐는데도 한번 더 실행되어 오류가 날 수 있다.

그냥 기억하자 high-level 건드린 적 있으면 fclose()만 사용해서 닫아라.


 <temporal file>:임시 파일
 char* tmpnam(char* s); // temp name
 >> 중복되지 않는 임시파일 명을 알아서 생성해서 s에 저장함
 >> 경로를 맘대로 못바꾸고, 보안상 취약점이 있어 쓰지 않는 걸 추천

 char* tempnam(const char* dir, const char* pfx);
 dir: 임시파일을 생성할 디렉토리 경로를 지정. 만약 NULL을 넘기거나 유효하지 않으면 시스템 기본 경로를 사용한다.
 pfx: 파일명 앞에 붙는 접두어. 최대 5글자.
 >>생성된 임시파일명은 malloc으로 동적할당되어 저장된다. 따라서 나중에 free()해줘야 한다.

 하지만 이름만 생성해주기 때문에 파일을 여는건 직접 해야한다. 하지만 그 찰나의 사이 순간에 여러개의 프로세스들이 동시에 같은 이름을
 가지고 파일을 만들려하는 순간 위험하기 때문에.. 이름생성과 파일 오픈을 동시에 해주는 mkstemp()를 쓰는걸 권장한다.


FILE* tmpfile(); //실패시 NULL 
>> 중복되지 않는 임시 파일을 생성하고 파일 포인터를 리턴한다. 그 파일은 "wb+"모드로 열린다.
>> 자동 삭제라는 특징을 갖고있다. fclose로 수동으로 닫아도 되지만, 프로그램이 정상적으로 종료된다면 알아서 삭제된다.


student_info.c 참고
scanf("%s\n", buf); 이경우 스트링을 받을 때 개행문자를 만나도 계속 진행한다. 즉 공백문자가 아닌문자가 나오기 전까지 계속 받는것이다.
따라서 의도대로 작동하지 않는다. 이 경우 \n을 지우고 그 다음 줄에 getchar();로 개행문자를 비워냄으로써 정상작동할 수 있다.
    