#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <boost/tokenizer.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

auto read_json_lines(const string& filepath) -> vector<string> {
	auto lines = vector<string>{};
	auto file = ifstream(filepath);
	if (!file.is_open()) {
		throw runtime_error("No se pudo abrir el archivo: " + filepath);
	}

	auto line = string{};
	while (getline(file, line)) {
		lines.push_back(line);
	}
	return lines;
}

auto count_words(const string& text) -> ptrdiff_t {
	auto tokenizer = boost::tokenizer<boost::char_separator<char>>{
		text, boost::char_separator<char>{" ", "", boost::keep_empty_tokens}
	};
	return distance(tokenizer.begin(), tokenizer.end());
}

//////////////////////////////////////////////////////////////////////////////////////

auto main() -> int 
{
    const string filepath = "./amazon_software_5.json";

    auto product_reviews = map<string, vector<ptrdiff_t>>{};
    auto product_avg_lengths = map<string, double>{};
    auto avg_length_to_products = map<double, vector<string>, ranges::greater>{};

    try {
        auto json_lines = read_json_lines(filepath);

        for (const auto& line : json_lines) {
            try {
                auto j = json::parse(line);
                if (j.contains("asin") && j.contains("reviewText")) {
                    const auto& asin = j["asin"].get<string>();
                    const auto& review_text = j["reviewText"].get<string>();

                    auto word_count = count_words(review_text);

                    product_reviews[asin].push_back(word_count);
                }
            } catch (const json::exception& e) {
                continue;
            }
        }

        for (const auto& [asin, lengths] : product_reviews) {
            double avg_length = accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
            product_avg_lengths[asin] = avg_length;
        }

        for (const auto& [asin, avg_length] : product_avg_lengths) {
            avg_length_to_products[avg_length].push_back(asin);
        }

        cout << "Los 5 primeros productos con longitudes medias de opiniones:" << endl;
        for (const auto& [asin, avg_length] : product_avg_lengths | views::take(5)) {
            cout << "Producto: " << asin << " -> Longitud media: " << avg_length << endl;
        }

        cout << endl << "Top 5 longitudes medias con sus productos asociados:" << endl;
        for (const auto& [avg_length, products] : avg_length_to_products | views::take(5)) {
            cout << "Longitud media: " << avg_length << " -> Productos: ";
            for (const auto& asin : products) {
                cout << asin << " ";
            }
            cout << endl;
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
