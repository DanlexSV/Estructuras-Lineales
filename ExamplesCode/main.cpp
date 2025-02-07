// ficheros de cabecera estándar:
#include <algorithm>  // contiene algortimos de búsqueda, partición, ordenación, etc
#include <filesystem> // sistema de ficheros
#include <fstream>    // flujos i/o a ficheros
#include <generator>  // generador asociado a corrutina
#include <print>      // contiene las funciones std::print y std::println
#include <ranges>     // contiene vistas como std::views::chunk_by
#include <string>     // cadena de caracteres estándar
#include <vector>     // contenedor secuencial recomendado por defecto en C++

// biblioteca no-estádar:
#include <nlohmann/json.hpp>

// información relevante de un objetivo:
struct Target {
    std::string name; // tipo de objetivo
    bool achieved;    // ¿está el target completado?   
};

// macro asociada de la biblioteca NLohmann's JSON:
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Target, name, achieved)

////////////////////////////////////////////////////////////////////////////////////////////////////

// corrutina que proporciona una secuencia de elementos generados a demanda, suspendiendo y
// retomando repetidamente su ejecución:
template<typename T>
auto get_from_jsonl(std::filesystem::path pth) -> std::generator<T>
{
    // si el fichero no tiene la extensión correcta, lanzamos una excepción:
    if (pth.extension() != ".jsonl") {
        throw std::ios::failure{"the extension of the JSON Lines file must be '.jsonl'"};
    }

    // creamos un flujo de entrada al fichero JSON Lines:
    auto ifs = std::ifstream{pth, std::ios::binary};
    // si el flujo no está abierto, lanzamos una excepción:
    if (!ifs) {
        throw std::ios::failure{"unable to open the JSON Lines file"};
    }

    // deserializamos la información del fichero línea a línea:
    auto ln = std::string{};
    while (std::getline(ifs, ln)) {
        // parseamos la línea y retornamos un valor de tipo T, suspendiendo momentáneamente la
        // ejecución de la corrutina:
        co_yield nlohmann::json::parse(ln).get<T>();
    }
} // el flujo al fichero es cerrado automáticamente por el destructor de std::ifstream

auto main() -> int
try {
    // alias para espacios de nombres:
    namespace stdr = std::ranges;
    namespace stdv = std::views;

    // construimos un vector de objetos Target a partir del fichero JSON Lines:
    auto targets = get_from_jsonl<Target>("../../military_camp.jsonl") | stdr::to<std::vector>();
    
    // ordenamos el vector de Targets por orden lexicográfico de "name":
    stdr::sort(targets, {}, &Target::name);

    // lambda auxiliar:
    auto same_name = [](Target const& t1, Target const& t2){ return t1.name == t2.name; };

    // iteramos los subgrupos del vector de targets de idéntico nombre:
    for (auto target_chunk : targets | stdv::chunk_by(same_name)) {
        auto const& name = stdr::begin(target_chunk)->name; // nombre del primer representante (referencia & para evitar copia)
        auto const achv = stdr::count_if(target_chunk, &Target::achieved);
        auto const totl = stdr::distance(target_chunk);
        std::println("{:>15} --> {}/{}", name, achv, totl);
    }

    return EXIT_SUCCESS;
}
catch (std::exception const& e) {
    std::print("{}", e.what());
    return EXIT_FAILURE;
}
catch (...) {
    std::print("unexpected exception");
    return EXIT_FAILURE;
}