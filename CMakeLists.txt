cmake_minimum_required(VERSION 3.28)

project(acreed
   VERSION 0.1.0
   DESCRIPTION "Proyecto sencillo de C++ con un solo target"
   LANGUAGES CXX
)

# Crear el ejecutable
add_executable(${PROJECT_NAME} main.cpp)

# Definir la versión de C++ que se usará
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Establecer propiedades del ejecutable
set_target_properties(${PROJECT_NAME} PROPERTIES
   CXX_EXTENSIONS OFF
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
)

#-------------------------------------------------------------------
# integración de bibliotecas de terceros:

find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)

#-------------------------------------------------------------------
# política de avisos:

option(ENABLE_WARNINGS_AS_ERRORS "Tratar advertencias como errores" ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
   target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
   if (ENABLE_WARNINGS_AS_ERRORS)
      target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
   else()
      target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
   endif()
endif()

#-------------------------------------------------------------------
# mensajes a emitir en construcción:

message("-- Generador de CMake: ${CMAKE_GENERATOR}")
message("-- Proyecto: ${PROJECT_NAME}, versión ${PROJECT_VERSION}")

get_target_property(CFEATURES ${PROJECT_NAME} COMPILE_FEATURES)
message("-- Target compile features: ${CFEATURES}")

get_target_property(COPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
message("-- Target compile options: ${COPTIONS}")
