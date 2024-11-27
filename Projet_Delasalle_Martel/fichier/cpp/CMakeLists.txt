cmake_minimum_required(VERSION 3.9)

project("Carrefour")

# Définir les versions de C++ standard souhaitées
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter les fichiers sources du projet
set(SOURCES
    main.cpp
    TrafficLight.cpp
    Voiture.cpp
)

set(HEADERS
    TrafficLight.hpp
    Voiture.hpp
)

# Créer l'exécutable à partir des fichiers sources
add_executable(Carrefour ${SOURCES} ${HEADERS})

# Trouver le package SFML et les bibliothèques nécessaires
find_package(SFML 2.5 COMPONENTS window system graphics REQUIRED)

# Lier l'exécutable avec les bibliothèques SFML
target_link_libraries(Carrefour sfml-graphics sfml-window sfml-system)

# Ajouter le support pour les threads
find_package(Threads REQUIRED)
target_link_libraries(Carrefour Threads::Threads)

# Ajouter une condition pour CMake version > 3.12 pour C++20
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Carrefour PROPERTY CXX_STANDARD 20)
endif()

# (Optionnel) Ajouter des cibles pour l'installation et les tests si nécessaire
