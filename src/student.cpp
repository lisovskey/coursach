#include <iostream>
#include <fstream>
#include <string>
#include "student"
using namespace std;

typedef struct {
	int math;
	int prog;
	int phys;
	int phil;
} marks;

typedef struct {
	bool graphics;
	bool english;
	bool swimming;
	bool designing;
	bool history;
} credits;

typedef struct {
	string name;
	int group;
	marks knowledge;
	double gpa;
	credits pass;
	bool budget;
	bool social;
	double cash;
} student;

void addStudent() {}

void findStudent() {}

void editStudent() {}

void sortStudents() {}

void saveChanges() {}
