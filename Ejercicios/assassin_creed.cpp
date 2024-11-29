#include <algorithm>
#include <iostream>
#include <fstream>
#include <generator>
#include <string>
#include <vector>
#include <ranges>
#include <format>

// Librerias no Estandar
#include <nlohmann/json.hpp>

using namespace std;

struct Target {
	string name;
	bool achieved;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Target, name, achieved)

///////////////////////////////////////////////////////////////////////
	
	template<typename T>
	auto get_from_jsonl(string file) -> generator<T> {
		auto ifs = ifstream{file, ios::binary};

		if (!ifs) {
			throw ios::failure{"Unable to open the JSON LINES file"};
		}

		auto ln = string{};
		while (getline(ifs, ln)) {
			auto trgt = nlohmann::json::parse(ln).get<T>();
			co_yield trgt;
		}
	}

template<typename T>
struct Rectangle {
	T width;
	T height;
};

auto main() -> int {
	auto r1 = Rectangle{5.6, 1.23};
	auto r2 = Rectangle{7.87f, 100.2f};

	cout << format("base: {}, altura: {}", r1.width, r1.height) << endl;
	cout << format("base: {}, altura: {}", r2.width, r2.height) << endl;


	auto targets = vector<Target>{};

	for (Target trgt : get_from_jsonl<Target>("../military_camp.jsonl")) {
		targets.push_back(trgt);
	}

	ranges::sort(targets, {}, &Target::name);

	auto same_name = [](Target const& t1, Target const& t2){ return t1.name == t2.name; };

	for (auto target_chunk : targets | views::chunk_by(same_name)) {
		auto const name = ranges::begin(target_chunk)->name;
		auto const achv = ranges::count_if(target_chunk, &Target::achieved);
		auto const totl = ranges::distance(target_chunk);
		cout << format("{:>15} --> {}/{}", name, achv, totl) << endl;
	}

	return EXIT_SUCCESS;
} 
