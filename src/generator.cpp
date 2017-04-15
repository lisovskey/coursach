/*
* RitZEED inc.
*/

#include "stdafx"
#include "generator"
#include <random>
using namespace std;

const string NAMES[] = { "Alex", "Anton", "Alice", "Anastasia", "Alena", "Angella",
"Alina", "Anatoly", "Andrew", "Alexander", "Alexandra", "Artur", "Boris", "Caroline",
"Christine", "Dmitry", "Danila", "Denis", "Daria", "Evgeny", "Evgeniya", "Egor",
"Elisabet", "Eveline", "Elya", "Eldar", "Fedor", "German", "Gleb", "Grigory", "Georgy",
"Gennady", "Galina", "Hanna", "Helen", "Igor", "Irina", "Ira", "Kate", "Kseniya",
"Karina", "Kirill", "Konstantin", "Leonid", "Lubov", "Larisa", "Ludmila", "Lina",  "Maxim",
"Matvey", "Miron", "Margaret", "Mary", "Nikita", "Nikolai", "Nadejda", "Natali", "Nina",
"Oleg", "Oksana", "Olga", "Olesya", "Peter", "Pavel", "Polina", "Roma", "Ruslan", "Rodion",
"Rose", "Sergey", "Semen", "Sonya", "Sophie", "Svetlana", "Stanislav", "Timofey", "Timur",
"Tatuyana", "Uluyana", "Vadim", "Valentin", "Valentina", "Valery", "Valeria", "Vitaly",
"Victor", "Vladimir", "Vladislav", "Vasily", "Vyacheslav", "Vera", "Veronica", "Viktoriya",
"Vlada", "Violett", "Varvara", "Yury", "Yan", "Yana", "Yaroslav", "Zoe", "Zlata" };

const string SURNAMES[] = { "Black", "Gray", "White", "Green", "Brown", "Smith", "Martin",
"Wilson", "Lee", "Johnson", "Williams", "Miller", "Jones", "Davis", "Anderson", "Taylor",
"Thomas", "Jackson", "Harris", "Lewis", "Robinson", "Walker", "Young", "Allen", "Nelson",
"Scott", "Hill", "King", "Mitchell", "Roberts", "Carter", "Adams", "Evans", "Turner", "Diaz" };

const size_t GROUPS[] = { 
	651001, 651002, 651003, 651004, 651005, 651006, 650501,	650502, 650503, 650504, 650505, 
	662101, 644101, 661401, 630501, 653501, 653502, 653503,	653504, 653505, 674001, 674002, 
	674003, 674004, 610901, 642801, 614301, 614302, 650701,	660801, 660802, 672301, 672302, 
	672303, 673901, 673902, 673903, 673904, 610101, 634201,	633701, 633702, 663101, 663102, 
	673601, 673602, 640301, 622401, 622102, 622403, 624401,	624402, 643201, 621901, 620601, 
	620602, 620603, 620604, 641301, 640401, 621701, 621702,	681065, 622402, 642791, 641591, 
	641291, 644691, 644692, 611801, 611101, 610201, 612601,	613301, 613801, 613802, 600541, 
	602441, 603341, 603041, 600841, 600441, 600241, 693851,	680561, 680562, 680961, 681061, 
	681062, 681063, 681064 
};

random_device rd;
mt19937 mt(rd());

string generateName()
{
	string name = "";
	uniform_int_distribution<int> dist_name(0, 101);
	name += NAMES[dist_name(mt)];
	name += " ";
	uniform_int_distribution<int> dist_surname(0, 34);
	name += SURNAMES[dist_surname(mt)];
	return name;
}

size_t generateGroup()
{
	uniform_int_distribution<int> dist(0, 90);
	return GROUPS[dist(mt)];
}

size_t generateMark()
{
	uniform_int_distribution<int> dist(4, 10);
	return dist(mt);
}

bool generateBool(const int factor)
{
	uniform_int_distribution<int> dist(0, abs(factor));
	return factor >= 0 ? dist(mt) != 0 : dist(mt) == 0;
}
