#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <map>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>

using namespace std;

auto file_to_string(string filename) -> string
{
	auto in = ifstream{};
	in.exceptions(ifstream::failbit | ifstream::badbit);
	in.open(filename, ios::binary);

	auto contents = string{};
	in.seekg(0, ios::end);
	if (auto const sz = streamoff{in.tellg()}; sz > 0) {
		contents.resize(sz);
		in.seekg(0, ios::beg);
		in.read(&contents[0], sz);
	}
	return contents;
}

////////////////////////////////////////////////////////////////////////////////////////////////

auto main() -> int
{
	auto text = file_to_string("../TXT/ulysses_joyce.txt");
	ranges::transform(text, text.begin(), [](unsigned char c) { return tolower(c); });

	auto tokens = ranges::to<vector<string>>(boost::tokenizer{text});
	ranges::sort(tokens);

	auto freq_tokens_map = map<int, vector<string>, ranges::greater>{};

	for (
			auto same_token = [](string const& t1, string const& t2) { return t1 == t2; }; 
			auto token_chunk : tokens | views::chunk_by(same_token)
		) {
		auto const freq = ranges::distance(token_chunk);
		auto const tkn = *ranges::begin(token_chunk);

		freq_tokens_map[freq].push_back(tkn);
	}

	for (auto [freq, tokens] : freq_tokens_map | views::take(5)) {
		cout << freq << " --> ";

		for (string tkn : tokens) {
			cout << tkn;
		}

		cout << endl;
	}

	return EXIT_SUCCESS;
}
