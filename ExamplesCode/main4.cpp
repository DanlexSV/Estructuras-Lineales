// ficheros de cabecera estándar:
#include <cmath>     // funciones matemáticas
#include <cstdlib>   // contiene las macros EXIT_SUCCESS y EXIT_FAILURE
#include <algorithm> // contiene algortimos de búsqueda, partición, ordenación, etc
#include <print>     // contiene las funciones std::print y std::println
#include <random>    // generadores de número aletatorios, distribuciones
#include <string>    // cadena de caracteres estándar
#include <vector>    // contenedor secuencial recomendado por defecto en C++

struct Point {
   double x;
   double y;
   double z;
};

struct Machine {
   Point location;
   bool alert;  // true si la distancia a Aloy es < 10.0m
   bool attack; // true si la distancia a Aloy es < 5.0m
};

auto main() -> int
{
   // alias para espacios de nombres:
   namespace stdr = std::ranges;

   // distancia de Aloy (situada en el origen de coordenadas) a un punto cualquiera del escenario:
   auto distance = [](Point const& p) -> double {
      auto const& [x, y, z] = p; // descomposición (structured binding)
      return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
   };

   auto gen = std::mt19937{std::random_device{}()}; // enteros aleatorios entre [0, 2^32)
   auto distr = std::uniform_real_distribution{-15.0, 15.0}; // [-15.0, 15.0)
   auto rand = [&]() -> double {
      return distr(gen);
   };

   // vector de máquinas inicialmente vacío:
   auto machines = std::vector<Machine>{};
   machines.reserve(25); // reservamos espacio para 25 máquinas

   // rellenamos el vector con máquinas en posición arbitraria:
   for (auto i = int{0}; i < 25; ++i) {
      auto const loc = Point{.x = rand(), .y = rand(), .z = rand()}; // punto donde situar a la máquina
      auto const dis = distance(loc); // distancia de la máquina a Aloy
      auto const m = Machine{.location = loc, .alert = dis < 10.0, .attack = dis < 5.0}; // máquina
      machines.push_back(m);
   }

   // particionamos el vector completo de forma que las máquinas en alerta se sitúen al frente;
   // alert_it señalará a la primera máquina que no esté en alerta;
   // el intervalo [alert_it, last_1) contiene a las máquinas que no están haciendo nada;
   // last_1 coincide con machines.end()
   auto [alert_it, last_1] = stdr::partition(machines, [](Machine const& m){ return m.alert; });

   // particionamos el subrango [machines.begin(), alert_it) de forma que las máquinas en ataque
   // se sitúen al frente; attack_it señala a la primera máquina que no está en ataque pero sí
   // en alerta (si existe); el intervalo [attack_it, last_2) contiene a las máquinas en alerta
   // pero no en ataque; last_2 coincide con alert_it
   auto [attack_it, last_2] = stdr::partition(stdr::subrange{machines.begin(), alert_it},
                                               [](Machine const& m){ return m.attack; });

   // imprimimos el subrango [machines.begin(), attack_it) de máquinas en ataque:
   std::println("Attacking_____________________");

   for (Machine const& m : stdr::subrange{machines.begin(), attack_it}) {
      std::println("x: {:.1f}, y: {:.1f}, z: {:.1f} | distance: {:.1f}m",
                    m.location.x, m.location.y, m.location.z, distance(m.location));
   }

   // imprimimos el subrango [attack_it, alert_it) de máquinas en alerta (pero no en ataque):
   std::println("On the alert__________________");

   for (Machine const& m : stdr::subrange{attack_it, alert_it}) {
      std::println("x: {:.1f}, y: {:.1f}, z: {:.1f} | distance: {:.1f}m",
                    m.location.x, m.location.y, m.location.z, distance(m.location));
   }

   return EXIT_SUCCESS;
}