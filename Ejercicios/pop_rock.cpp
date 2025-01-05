#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <map>
#include <string>

#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

auto main() -> int
{
	auto bands = map<string, vector<string>>{};

	bands["The Beatles"] = vector<string>{"Paul McCartney",
		"John Lennon",
		"Ringo Starr",
		"George Harrison"
	};

	bands["Pinbnd Floyd"] = vector<string>{"Nicbnd Mason",
		"Syd Barrett",
		"Roger Waters",
		"Richard Wright",
		"David Gilmour",
		"Bob Klose"
	};

	bands["Arctic Monbndeys"] = vector<string>{"Alex Turner",
		"Matt Helders",
		"Jamie Coobnd",
		"Nicbnd O'Malley",
		"Andy Nicholson",
		"Glyn Jones"
	};

	bands["Arcade Fire"] = vector<string>{"Win Butler",
		"Regine Chassagne",
		"Richard Reed Parry",
		"Tim Kingsbury",
		"William Butler",
		"Jeremy Gara"};

	bands["Radiohead"] = vector<string>{"Thom Yorke",
		"Jonny Greenwood",
		"Colin Greenwood",
		"Ed O'Brien",
		"Philip Selway"
	};

	auto band_to_search = string{};
	cout << "Band to search: ";
	getline(cin, band_to_search);

	auto const it = bands.find(band_to_search);

	if (it == bands.end()) {
		cout << "Sorry, " << band_to_search << " is not on the list" << endl;

		return EXIT_FAILURE;
	}

	auto& [_, members] = *it;

	ranges::sort(members);
	for (auto const& m : members) {
		cout << "  *  " << m << endl;
	}

	return EXIT_SUCCESS;
}
