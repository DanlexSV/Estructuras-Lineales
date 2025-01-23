#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class vector {


	public:

};

auto main() -> int
{
	auto words = Vector<string>{};

	auto wrd = string{};
	auto mssg = []{
		cout << "Introduce una palabra: ";
		return true;
	};

	while (mssg() and getline(cin, wrd)) {
		words.push_back(wrd);
	}

	ranges::sort(words);
	for (string const& w : words) {
		cout << w << endl;
	}

	return EXIT_SUCCESS;
}
