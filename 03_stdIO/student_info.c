#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NUM_STUDENT 255

enum action {ADD, FIND, EXIT};

typedef struct {
	int ID;
	char name[8];
	float score;
} Student; //total 16 bytes

int fileOpen(FILE** _fp, char* _filename, char* _mode);
int selectAction(void);
int printStudentInfo(Student* _info);
int addStudentInfo(FILE* _fp, Student* _info);
long findStudent(FILE* _fp, Student* _info);

int main(void){
	
}


int fileOpen(FILE** _fp, char* _filename, char* _mode)
int selectAction(void)
int printStudentInfo(Student* _info)
int addStudentInfo(FILE* _fp, Student* _info)
long findStudent(FILE* _fp, Student* _info)
