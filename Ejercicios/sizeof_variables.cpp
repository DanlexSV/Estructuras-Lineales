#include <iostream>
using namespace std;

auto main() -> int {
    auto f = float{8.7f};
    auto d = double{7.9};
    auto c = char{'a'};

    cout << "sizeof f = " << sizeof(f) << " bytes, "
         << "d = " << sizeof(d) << " bytes, "
         << "c = " << sizeof(c) << " bytes" << endl;

    return 0;
}

