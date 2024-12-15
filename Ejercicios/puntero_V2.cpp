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
	Student* p = new Student[100]{};

	(*(p + 3)).grade_1 = 9.75;
	(p + 3)->grade_2 = 8.88;
	p[3].grade_3 = 10;

	cout << "La nota media es: " << fixed << setprecision(2) << p[3].average() << endl;

	delete[] p;

	return EXIT_SUCCESS;
}
