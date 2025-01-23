#include <iostream>
#include <vector>
#include <random>
#include <future>       // std::async, std::future
#include <thread>       // std::thread::hardware_concurrency
#include <ranges>       // std::ranges::max_element
#include <algorithm>    // Para std::max_element si hiciera falta
#include <cstdint>
#include <chrono>
#include <limits>

using namespace std;

using uf32 = std::uint_fast32_t;

int main()
{
    // Para medir tiempo de ejecución
    using reloj = std::chrono::high_resolution_clock;
    auto t0 = reloj::now();

    // -------------------------------------------------------------------------
    // 1. Preparar vector de 100.000.000 de uf32 con valores aleatorios
    // -------------------------------------------------------------------------
    const std::size_t N = 100'000'000;
    std::vector<uf32> datos(N);

    // Generador de números aleatorios (Mersenne Twister)
    std::random_device rd;
    std::mt19937 gen(rd());

    // Distribución uniforme en [0, max de uf32]
    std::uniform_int_distribution<uf32> dist(
        std::numeric_limits<uf32>::min(),
        std::numeric_limits<uf32>::max()
    );

    // Rellenar el vector con valores aleatorios
    for (auto &elem : datos) {
        elem = dist(gen);
    }

    // -------------------------------------------------------------------------
    // 2. Dividir el vector según hilos disponibles y lanzar tareas asíncronas
    // -------------------------------------------------------------------------
    unsigned numHilos = std::thread::hardware_concurrency();
    if (numHilos == 0) {
        numHilos = 1;  // fallback si hardware_concurrency() devuelve 0
    }

    std::cout << "Lanzando " << numHilos << " hilos...\n";

    std::vector<std::future<uf32>> futures;
    futures.reserve(numHilos);

    // Tamaño de bloque para cada hilo
    std::size_t bloque = N / numHilos;
    std::size_t resto = N % numHilos;

    std::size_t inicio = 0;
    for (unsigned i = 0; i < numHilos; ++i) {
        std::size_t fin = inicio + bloque + ((i < resto) ? 1 : 0);

        // Lanzamos una tarea asíncrona para buscar el máximo en [inicio, fin)
        futures.push_back(
            std::async(std::launch::async,
                       [inicio, fin, &datos]() -> uf32 {
                           // C++20/23: std::ranges::max_element
                           auto it = ranges::max_element(
                               datos.begin() + inicio,
                               datos.begin() + fin
                           );
                           return (it != (datos.begin() + fin)) ? *it : 0;
                       })
        );

        inicio = fin;  // avanzamos para el siguiente hilo
    }

    // -------------------------------------------------------------------------
    // 3. Recoger resultados parciales y buscar el máximo global
    // -------------------------------------------------------------------------
    std::vector<uf32> maxParciales(numHilos);

    for (unsigned i = 0; i < numHilos; ++i) {
        maxParciales[i] = futures[i].get();  // valor máximo parcial
    }

    // Última operación para encontrar el máximo global
    uf32 maxGlobal = *std::ranges::max_element(maxParciales);

    // -------------------------------------------------------------------------
    // 4. Imprimir resultado y tiempo de ejecución
    // -------------------------------------------------------------------------
    auto t1 = reloj::now();
    std::chrono::duration<double> duracion = (t1 - t0);

    std::cout << "Valor maximo en el vector: " << maxGlobal << '\n';
    std::cout << "Tiempo de ejecucion: " << duracion.count() << " s\n";

    return 0;
}
