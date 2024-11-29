#include <algorithm>
#include <iostream>
#include <fstream>
#include <generator>
#include <string>
#include <vector>
#include <ranges>
#include <format>

// Librería no estándar
#include <nlohmann/json.hpp>

using namespace std;

struct Mission {
	string district;
	string type;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Mission, district, type)

/////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	auto get_from_jsonl(string file) -> generator<T> {
		auto ifs = ifstream{file, ios::binary};
		if (!ifs) {
			throw ios::failure{"Unable to open the JSONL file"};
		}

		auto line = string{};
		while (getline(ifs, line)) {
			auto obj = nlohmann::json::parse(line).get<T>();
			co_yield obj;
		}
	}

template <auto Field>
struct CompareBy {
	bool operator()(const auto& a, const auto& b) const {
		return a.*Field == b.*Field;
	}
};

int main() {
	auto missions = vector<Mission>{};

	for (auto const mission : get_from_jsonl<Mission>("./peter_parker.jsonl")) {
		missions.push_back(mission);
	}

	ranges::sort(missions, {}, &Mission::district);

	auto const by_district = CompareBy<&Mission::district>{};
	auto const by_type = CompareBy<&Mission::type>{};

	for (auto district_chunk : missions | views::chunk_by(by_district)) {
		auto const district_name = ranges::begin(district_chunk)->district;
		cout << district_name << ":" << endl;

		ranges::sort(district_chunk, {}, &Mission::type);

		for (auto type_chunk : district_chunk | views::chunk_by(by_type)) {
			auto const mission_type = ranges::begin(type_chunk)->type;
			auto const count = ranges::distance(type_chunk);

			cout << format("  * {:<20} -> {}", mission_type, count) << endl;
		}
	}

	return EXIT_SUCCESS;
}
