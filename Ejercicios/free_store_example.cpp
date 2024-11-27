#include <iostream>
#include <string>
#include <vector>

using namespace std;

auto main() -> int {
    auto names = std::vector<std::string>{}; // vector inicialmente vacío
    auto nm = std::string{}; // string inicialmente vacío

    cout << "Introduce un nombre: " << endl;

    while (std::getline(std::cin, nm)) {
        names.push_back(nm);
        cout << "Introduce un nombre: " << endl;
    }

    for (std::string n : names) {
        cout << n << endl;
    }
}
