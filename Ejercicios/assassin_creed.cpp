#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <nlohmann/json.hpp>
#include <algorithm>

using namespace std;

struct Target {
    string name;
    bool achieved;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Target, name, achieved)

auto same_name(Target t1, Target t2) -> bool {
    return t1.name == t2.name;
}

auto main() -> int {
    auto targets = vector<Target>{};

    auto ifs = ifstream{"../military_camp.jsonl", ios::binary};
    if (!ifs) {
        cout << "Incapaz de abrir el fichero JSON Lines" << endl;
        return EXIT_FAILURE;
    }

    auto ln = string{};
    while (getline(ifs, ln)) {
        auto trgt = nlohmann::json::parse(ln).get<Target>();
        targets.push_back(trgt);
    }
    ifs.close();

    ranges::sort(targets, {}, &Target::name);

    auto grouped_targets = vector<vector<Target>>{};
    for (const auto& target : targets) {
        if (grouped_targets.empty() || grouped_targets.back().front().name != target.name) {
            grouped_targets.push_back({});
        }
        grouped_targets.back().push_back(target);
    }

    for (const auto& group : grouped_targets) {
        cout << "tipo: " << group.front().name << endl;
    }

    return EXIT_SUCCESS;
}

