#include <iostream>
#include <algorithm>   // std::ranges::max_element en C++23
#include <chrono>      // std::chrono para medir tiempos
#include <cmath>       // funciones matemáticas
#include <cstdlib>     // EXIT_SUCCESS
#include <cstdint>     // std::uint_fast32_t
#include <future>      // std::async, std::future
#include <print>       // std::print, std::println (C++23)
#include <random>      // std::mt19937, std::uniform_int_distribution
#include <ranges>      // std::ranges
#include <thread>      // std::thread::hardware_concurrency
#include <vector>      // std::vector

// Alias para evitar escribir std::uint_fast32_t
using uf32 = std::uint_fast32_t;

// -----------------------------------------------------------------------------
// Función que genera el gran vector de N elementos con valores aleatorios
// -----------------------------------------------------------------------------
auto generar_datos(std::size_t N) -> std::vector<uf32>
{
   auto datos = std::vector<uf32>{};
   datos.reserve(N);

   // Generador Mersenne Twister y distribución uniforme en [0, max(uf32)]
   std::random_device rd;
   std::mt19937 gen{ rd() };
   std::uniform_int_distribution<uf32> dist(
      std::numeric_limits<uf32>::min(),
      std::numeric_limits<uf32>::max()
   );

   // Rellenamos el vector
   for (std::size_t i = 0; i < N; ++i) {
      datos.push_back(dist(gen));
   }

   return datos;
}

// -----------------------------------------------------------------------------
// Versión SINGLE THREAD: encontrar el máximo con un solo hilo
// -----------------------------------------------------------------------------
auto max_single_thread(std::vector<uf32> const& datos) -> uf32
{
   // std::ranges::max_element (C++23).
   auto it = std::ranges::max_element(datos);
   return (it != datos.end()) ? *it : uf32{0};
}

// -----------------------------------------------------------------------------
// Versión MULTI-THREAD: dividir el trabajo según el número de hilos disponibles
// -----------------------------------------------------------------------------
auto max_multi_thread(std::vector<uf32> const& datos) -> uf32
{
   // 1) Averiguamos cuántos hilos podemos lanzar
   unsigned numHilos = std::thread::hardware_concurrency();
   if (numHilos == 0) {
      numHilos = 1; // fallback
   }

   // 2) Tamaño del vector y dimensiones de partición
   auto const N = datos.size();
   auto const bloque = N / numHilos;
   auto const resto = N % numHilos;

   // 3) Creamos futures (uno por cada subrango)
   auto futures = std::vector<std::future<uf32>>{};
   futures.reserve(numHilos);

   // 4) Para cada hilo, calculamos inicio y fin del subrango
   auto inicio = std::size_t{0};
   for (unsigned i = 0; i < numHilos; ++i) {
      auto fin = inicio + bloque + (i < resto ? 1 : 0);

      // Lanza la tarea asíncrona
      futures.push_back(
         std::async(std::launch::async,
                    [inicio, fin, &datos]() -> uf32 {
                       // En C++23 podemos usar std::ranges::max_element
                       auto it = std::ranges::max_element(
                           datos.begin() + inicio,
                           datos.begin() + fin
                       );
                       return (it != datos.begin() + fin) ? *it : uf32{0};
                    })
      );

      inicio = fin;
   }

   // 5) Recoger los resultados parciales y hallar el max global
   uf32 maxGlobal = 0;
   for (auto &f : futures) {
      auto parcial = f.get();
      if (parcial > maxGlobal) {
         maxGlobal = parcial;
      }
   }

   return maxGlobal;
}

// -----------------------------------------------------------------------------
// main() - Genera los datos, mide tiempos en single-thread y multi-thread
// -----------------------------------------------------------------------------
auto main() -> int
{
   // 1) Tamaño del vector
   auto const N = std::size_t{100'000'000};

   // 2) Generar datos (100 millones de elementos)
   std::cout << "Generando " << N << " elementos aleatorios..." << std::endl;
   auto datos = generar_datos(N);

   // 3) SINGLE THREAD
   {
      using reloj = std::chrono::high_resolution_clock;
      auto t0 = reloj::now();

      auto resultado = max_single_thread(datos);

      auto t1 = reloj::now();
      std::chrono::duration<double> dur = t1 - t0;

	  std::cout << "[SINGLE THREAD] Max: " << resultado << std::endl;
	  std::cout << "[SINGLE THREAD] Tiempo: " << std::fixed << std::setprecision(2) << dur.count() << " s" << std::endl;
   }

   // 4) MULTI-THREAD
   {
      using reloj = std::chrono::high_resolution_clock;
      auto t0 = reloj::now();

      auto resultado = max_multi_thread(datos);

      auto t1 = reloj::now();
      std::chrono::duration<double> dur = t1 - t0;

	  std::cout << "[MULTI THREAD] Max: " << resultado << std::endl;
	  std::cout << "[MULTI THREAD] Tiempo: " << std::fixed << std::setprecision(2) << dur.count() << " s" << std::endl;
   }

   return EXIT_SUCCESS;
}

