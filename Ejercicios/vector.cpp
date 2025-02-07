#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>

using namespace std;

template<typename T>
class vector {
	T* v_;
	T* space_;
	T* last_;

	public:
	Vector()
		: v_{new T[0]}, space_{v_}, last_{v_}
	{ }


	~Vector()
	{
		delete[] v_;
	}

	// Funciones de capacidad -----------------------
	auto size() -> size_t { return space_ - v_; }
	auto capacity() -> size_t { return last_ - v_; }
	auto empty() -> bool { return v_ == space_; }

	// Funciones de acceso --------------------------
	auto operator[](size_t idx) -> T& { return v_[idx]; }

	auto at(size_t idx) -> T&
	{
		if (idx >= size()) {
			throw out_of_range{};
		}
		return v_[idx];
	}

	auto begin() -> T* { return v_; }
	auto end() -> T* { return space_; }

	// Insertar nuevos elementos al fondo del vector
	auto push_back(T val) -> void
	{
		if (space_ == last_) {
			auto cp = capacity();
			auto new_cp = (cp == 0) ? size_t{2} : 2 * cp;
			
			T* new_block = new T[new_cp];
			try {
				for (auto i = size_t{0}; i < cp; ++i) {
					new_block[i] = v_[i];
				}

				new_block[cp] = val;
			}
			catch () {
				delete[] new_block;
				throw;
			}
		}
		else {

		}
	}
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
