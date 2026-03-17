#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NUM_STUDENT 255

enum action {ADD, FIND, EXIT};

typedef struct {
	int ID;
	char name[16];
	float score;
} Student; //total 24 bytes

int fileOpen(FILE** _fp, char* _filename, char* _mode);

int selectAction(void);

void printStudentInfo(Student* _info);

void addStudentInfo(FILE* _fp, Student* _info);

long findStudent(FILE* _fp, Student* _info);

int main(void){
	FILE* fp = NULL;
	Student data = {0};
	fileOpen(&fp, "StudentDB", "ab+");

	while(1){
		switch(selectAction()){
		case ADD:
			addStudentInfo(fp, &data);
			break;
		case FIND:
			if(findStudent(fp, &data) < 0)
				printf("Cannot find the student\n");
			else
				printStudentInfo(&data);
			break;
		case EXIT:
			exit(0);
		
		}
	}
}

/*
파일 포인터의 주소, 파일 이름, 여는 모드를 인자로 받아 fopen을 호출한다.
만약 열기에 실패할 경우 -1, 성공하면 0을 리턴한다.
*/
int fileOpen(FILE** _fp, char* _filename, char* _mode){
	*_fp = fopen(_filename, _mode);
	if(!*_fp){
		printf("fail to open - %s\n", _filename);
		return -1;
	}
	return 0;	
}

//사용자로부터 ADD, FIND, EXIT중 하나를 선택하도록 인풋을 받는다.
//해당 enum값을 리턴한다.
int selectAction(void){
	int sel = 0;
	printf("[%d]add [%d]find [%d]exit: ", ADD, FIND, EXIT);
	scanf("%d", &sel);
	return sel;
}

//학생정보를 양식에 맞게 출력
void printStudentInfo(Student* _info){
	printf("%d %s %.2f\n", _info->ID, _info->name, _info->score);
}


void addStudentInfo(FILE* _fp, Student* _info){
	printf("enter student information\n");
	printf("ID name score: ");
	scanf("%d %s %f", &_info->ID, _info->name, &_info->score);
	//cf. scanf에서는 %.2f이런 양식을 쓰면 안된다.
	getchar(); //개행문자 비우기

	fseek(_fp, 0, SEEK_END);
	fwrite(_info, sizeof(Student), 1, _fp);
}

//사용자로부터 이름을 입력받고, 파일내에 동일한 이름이 있는지 비교한다.
//만약 찾았다면 해당 파일포인터의 위치를 반환하고, 못찾으면 -1을 리턴한다.
long findStudent(FILE* _fp, Student* _info){
	char name[16] = {0};
	printf("Name: ");
	scanf("%15s", name); //문자 최대 15개까지만 받는다.
	getchar();	

	size_t size = sizeof(Student);
	fseek(_fp, 0, SEEK_SET);

	//fread는 EOF같은 음수값을 반환하지 않는다.
	while(fread(_info, size, 1, _fp) == 1){
		if(strncmp(_info->name, name, 16) == 0){
			fseek(_fp, -(long)size, SEEK_CUR); //size_t를 long으로
			return ftell(_fp);
		}
	}
	return -1;

}
