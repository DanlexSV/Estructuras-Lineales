#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

auto main() -> int
{
    auto n = int{0};
    int* p1 = &n;

    auto d = double{9.9};
    double* p2 = &d;

    cout << "Puntero: " << p1 << " => " << sizeof(p1) << " bytes, puntero: " << p2 << " => " << sizeof(p2) << " bytes" << endl;
}
