#include <algorithm>
#include <cstdlib>   // contiene la función std::tolower (paso a minúscula)
#include <fstream>   // flujos I/O a ficheros
#include <map>       // contenedor asociativo de clave única ordenada
#include <print>
#include <ranges>
#include <string>
#include <vector>    // contenedor secuencial de elementos contiguos

#include <boost/tokenizer.hpp>

// carga en un string del contenido de un fichero:
auto file_to_string(std::string filename) -> std::string
{
   using namespace std;
   
   auto in = ifstream{};
   in.exceptions(ifstream::failbit | ifstream::badbit);
   in.open(filename, ios::binary);

   auto contents = string{};
   in.seekg(0, ios::end);
   if (auto const sz = streamoff{in.tellg()}; sz > 0) { // sz es entero con signo
      contents.resize(sz); // bloque de memoria contigua desde C++11
      in.seekg(0, ios::beg);
      in.read(&contents[0], sz);
   }
   return contents;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

auto main() -> int
{
   namespace stdr = std::ranges;
   namespace stdv = std::views;

   // cargamos el texto completo en memoria dentro de un string:
   auto text = file_to_string("../../ulysses_joyce_4300-0.txt");

   // pasamos a minúscula todos los caracteres del string:
   stdr::transform(text, text.begin(), [](unsigned char c){ return std::tolower(c); });

   // vector de strings que contiene todas las palabras del texto en el mismo orden en que aparecen
   // en la obra:
   auto tokens = stdr::to<std::vector<std::string>>(boost::tokenizer{text});
   
   // ordenamos el vector de tokens por orden alfabético:
   stdr::sort(tokens);

   // mapa con frecuencia de ocurrencia como clave y, como valor, el listado de palabras que se
   // repite edicho número de veces:
   auto freq_tokens_map = std::map<int, std::vector<std::string>, stdr::greater>{};

   // completamos el mapa con un bucle for-each que itere los sub-rangos del vector de palabras
   // idénticas:
   for (
      auto same_token = [](std::string const& t1, std::string const& t2){ return t1 == t2; };
      auto token_chunk : tokens | stdv::chunk_by(same_token)
   ) {
      auto const freq = stdr::distance(token_chunk);
      auto const& tkn = *stdr::begin(token_chunk); // & para evitar copia
      freq_tokens_map[freq].push_back(tkn);
   }

   // imprimimos en la terminal las cinco mayores frecuencias de ocurrencia acompañadas de sus
   // correspondientes listados de palabras:

   for (auto [freq, tokens] : freq_tokens_map | stdv::take(5)) {
      std::print("{} --> ", freq);
      for (std::string tkn : tokens) {
         std::print("{}, ", tkn);
      }
      std::println();
   }

   return EXIT_SUCCESS;
}