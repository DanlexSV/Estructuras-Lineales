#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <ranges>
#include <map>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>

#include <nlohmann/json.hpp>

using namespace std;

struct Point {
	double x;
	double y;
};

struct Character {
	string name;
	string town;
	Point location;
	bool leader;
};

void from_json(const nlohmann::json& j, Character& c) {
	j.at("name").get_to(c.name);
	j.at("town").get_to(c.town);

	auto coordinates = j.at("location").get<array<double, 2>>();
	c.location.x = coordinates[0];
	c.location.y = coordinates[1];

	j.at("leader").get_to(c.leader);
}

auto read_json_lines(const string& filepath) -> vector<string>
{
	vector<string> lines;
	ifstream file(filepath, ios::binary);

	if (!file.is_open()) {
		throw runtime_error("No se pudo abrir el archivo: " + filepath);
	}

	string line;
	while (getline(file, line)) {
		lines.push_back(line);
	}
	return lines;
}

auto distance_2d(const Point& a, const Point& b) -> double
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx*dx + dy*dy);
}


auto main() -> int
{
    try {
        constexpr int ZOMBIE_COUNT = 300;
        vector<Point> zombies(ZOMBIE_COUNT);

        mt19937 gen(random_device{}());
        uniform_real_distribution<double> distX(100.0, 1400.0);
        uniform_real_distribution<double> distY(150.0, 1100.0);

        for (auto &z : zombies) {
            z.x = distX(gen);
            z.y = distY(gen);
        }

		auto lines = read_json_lines("./walking_dead.jsonl");
        vector<Character> characters;
        characters.reserve(lines.size());

        for (auto &ln : lines) {
            try {
                nlohmann::json j = nlohmann::json::parse(ln);
                Character c = j.get<Character>();
                characters.push_back(c);
            }
            catch (...) {
                continue;
            }
        }

        ranges::sort(characters, {}, &Character::town);

        auto same_town = [](const Character& a, const Character& b) {
            return a.town == b.town;
        };

        for (auto &&town_group : characters | views::chunk_by(same_town)) {
            vector<Character> town_vec(town_group.begin(), town_group.end());
            ranges::sort(town_vec, {}, &Character::name);

            const auto& town_name = town_vec.front().town;
            int population = static_cast<int>(town_vec.size()) * 10;
            auto leader_it = ranges::find_if(town_vec, &Character::leader);

            int dangerously_close_walkers = 0;
            if (leader_it != town_vec.end()) {
                auto &leader_loc = leader_it->location;
                dangerously_close_walkers = ranges::count_if(zombies, [&](const Point& z){
                    return distance_2d(z, leader_loc) < 250.0;
                });
            }

            cout << "-----------" << town_name << "-----------" << endl;
            for (auto &ch : town_vec) {
                cout << ch.name
                     << " | x: " << ch.location.x
                     << ", y: " << ch.location.y;
                if (ch.leader) {
                    cout << " [[leader]]";
                }
                cout << endl;
            }
            cout << "Population: " << population << endl;
            cout << "Number of dangerously close walkers: "
                 << dangerously_close_walkers << endl << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Excepción capturada: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

