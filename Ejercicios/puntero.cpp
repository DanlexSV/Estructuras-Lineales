#include <iostream>
#include <string>
#include <iomanip>
#include <memory>

using namespace std;

struct Student {
	string name;
	double grade_1 = 0, grade_2 = 0, grade_3 = 0;

	auto average() -> double { return (grade_1 + grade_2 + grade_3) / 3; }
};

auto main() -> int
{
	Student* s = new Student{"James Smith", 7.5, 8, 9.5};

	// Puntero unico
	auto p = make_unique<Student>();
	
	// Puntero compartido
	auto ps = make_shared<Student>();

	cout << fixed << setprecision(2) << s->grade_1 << endl;
	cout << fixed << setprecision(2) << s->average() << endl;

	delete s;

	return EXIT_SUCCESS;
}
