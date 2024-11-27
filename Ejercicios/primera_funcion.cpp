#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;

void incremento(double& v)
{
    v += 1.2;
}

int main()
{
    double d = 7.8;

    incremento(d);

    cout << "d = " << d << endl;

    return 0;
}

