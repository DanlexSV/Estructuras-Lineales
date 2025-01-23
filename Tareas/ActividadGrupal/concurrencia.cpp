#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <thread>
#include <future>
#include <ranges>

using namespace std;

using uf32 = std::uint_fast32_t;

// Función para ejecutar con paralelismo
uf32 max_paralelismo(std::vector<uf32>& datos, std::size_t N)
{
    unsigned numHilos = std::thread::hardware_concurrency();
    if (numHilos == 0) {
        numHilos = 1;  // fallback si hardware_concurrency() devuelve 0
    }

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
                        auto it = std::ranges::max_element(
                            datos.begin() + inicio,
                            datos.begin() + fin
                        );
                        return (it != (datos.begin() + fin)) ? *it : 0;
                    })
        );

        inicio = fin;  // avanzamos para el siguiente hilo
    }

    std::vector<uf32> maxParciales(numHilos);
    for (unsigned i = 0; i < numHilos; ++i) {
        maxParciales[i] = futures[i].get();
    }

    return *std::ranges::max_element(maxParciales);
}

// Función para ejecutar sin paralelismo
uf32 max_sin_paralelismo(std::vector<uf32>& datos)
{
    auto it = std::max_element(datos.begin(), datos.end());
    return (it != datos.end()) ? *it : 0;
}

int main()
{
    // Medir el tiempo de ejecución
    using reloj = std::chrono::high_resolution_clock;

    // Preparar el vector de 100.000.000 de uf32 con valores aleatorios
    const std::size_t N = 100'000'000;
    std::vector<uf32> datos(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uf32> dist(
        std::numeric_limits<uf32>::min(),
        std::numeric_limits<uf32>::max()
    );

    for (auto &elem : datos) {
        elem = dist(gen);
    }

    // -------------------------------------------------------------------------
    // Ejecución con paralelismo
    // -------------------------------------------------------------------------
    auto t0 = reloj::now();
    uf32 maxParalelo = max_paralelismo(datos, N);
    auto t1 = reloj::now();
    std::chrono::duration<double> duracionParalelismo = (t1 - t0);

    std::cout << "Valor maximo con paralelismo: " << maxParalelo << '\n';
    std::cout << "Tiempo de ejecucion con paralelismo: " << duracionParalelismo.count() << " s\n";

    // -------------------------------------------------------------------------
    // Ejecución sin paralelismo
    // -------------------------------------------------------------------------
    t0 = reloj::now();
    uf32 maxSecuencial = max_sin_paralelismo(datos);
    t1 = reloj::now();
    std::chrono::duration<double> duracionSecuencial = (t1 - t0);

    std::cout << "Valor maximo sin paralelismo: " << maxSecuencial << '\n';
    std::cout << "Tiempo de ejecucion sin paralelismo: " << duracionSecuencial.count() << " s\n";

    return 0;
}

