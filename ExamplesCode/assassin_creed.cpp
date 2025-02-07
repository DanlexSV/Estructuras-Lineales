#include <algorithm>
#include <filesystem>
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
	auto get_from_jsonl(filesystem::path pth) -> generator<T> {
		if (pth.extension() != ".jsonl") {
			throw ios::failure{"The extension of the JSON LINES file must be .jsonl"};
		}

		auto ifs = ifstream{pth, ios::binary};

		if (!ifs) {
			throw ios::failure{"Unable to open the JSON LINES file"};
		}

		auto ln = string{};
		while (getline(ifs, ln)) {
			co_yield nlohmann::json::parse(ln).get<T>();
		}
	}

auto main() -> int try {
	auto targets = get_from_jsonl<Target>("../military_camp.jsonl") | ranges::to<vector>();
	ranges::sort(targets, {}, &Target::name);

	auto same_name = [](Target const& t1, Target const& t2){ return t1.name == t2.name; };
	for (auto target_chunk : targets | views::chunk_by(same_name)) {
		auto const name = ranges::begin(target_chunk)->name;
		auto const achv = ranges::count_if(target_chunk, &Target::achieved);
		auto const totl = ranges::distance(target_chunk);
		cout << format("{:>15} --> {}/{}", name, achv, totl) << endl;
	}

	return EXIT_SUCCESS;
} catch (exception& e) {
	cout << e.what() << endl;
	return EXIT_FAILURE;
}
