#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

auto print_message(string const& str)
{
    cout << str << endl;
}

auto main() -> int
{
    auto mssg = string {"Hello, world!"};

    print_message(mssg);
    mssg += " OG!";

    cout << mssg << endl;
}
