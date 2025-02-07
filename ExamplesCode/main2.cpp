#include <algorithm>
#include <cstdlib>   // contiene las macros EXIT_SUCCESS y EXIT_FAILURE
#include <iostream>  // contiene los objetos globales std::cin y std::cout
#include <map>       // contenedor asociativo de clave única ordenada
#include <print>
#include <string>
#include <vector>    // contenedor secuencial recomendado por defecto

auto main() -> int
{
    // clave: nombre del grupo (string); valor asociado: integrantes del grupo (vector de string)
    auto bands = std::map<std::string, std::vector<std::string>>{}; // mapa inicialmente vacío

    bands["The Beatles"] = std::vector<std::string>{"Paul McCartney",
                                                    "John Lennon",
                                                    "Ringo Starr",
                                                    "George Harrison"};

    bands["Radiohead"] = std::vector<std::string>{"Thom Yorke",
                                                  "Jonny Greenwood",
                                                  "Colin Greenwood",
                                                  "Ed O'Brien",
                                                  "Philip Selway"};

    auto band_to_search = std::string{}; // inicialmente vacío
    std::print("Band to seach: "); // solicitamos al usuario la banda a buscar
    std::getline(std::cin, band_to_search); // registramos la banda en el string auxiliar desde la terminal

    if (bands.contains(band_to_search)) { // la banda solicitada está registrada como clave del mapa
        std::ranges::sort(bands[band_to_search]); // ordenamos los integrantes de la banda
        for (auto const& m : bands[band_to_search]) { // imprimimos a los integrantes de la banda
            std::println(" * {}", m);
        }
    }
    else { // la banda solicitada no está registrada en el mapa
        std::println("Sorry, '{}' is not on the list", band_to_search);
    }

    return 0;
}