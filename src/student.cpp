/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include "student"
using namespace std;

typedef struct {
	short	math;
	short	prog;
	short	phys;
	short	phil;
} marks;

typedef struct {
	bool	graphics;
	bool	english;
	bool	swimming;
	bool	designing;
	bool	history;
} credits;

struct student {
	string	name;
	int		group;
	marks	knowledge;
	double	gpa;
	credits pass;
	bool	budget;
	bool	social;
	double	cash;
};

void addStudent() {}

void findStudent() {}

void editStudent() {}

void viewStudents() {}

void sortStudents() {}

void saveChanges() {}
