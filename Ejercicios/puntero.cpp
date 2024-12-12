#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct Student {
	string name;
	double grade_1 = 0, grade_2 = 0, grade_3 = 0;

	auto average() -> double { return (grade_1 + grade_2 + grade_3) / 3; }
};

auto main() -> int
{
	auto s = Student{"James Smith", 7.5, 8, 9.5};

	cout << fixed << setprecision(2) << s.average() << endl;

	return EXIT_SUCCESS;
}
