cmake_minimum_required (VERSION 3.9)

project("Carrefour2")

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Carrefour2 "main.cpp" "Voiture.cpp" "Voiture.hpp" "tricolore.hpp")

target_link_libraries(Carrefour2 sfml-graphics sfml-window)

find_package (Threads)
find_package(SFML 2.5 COMPONENTS window system graphics REQUIRED)

link_libraries (Threads::Threads)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Carrefour2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.