cmake_minimum_required (VERSION 3.9)

project(thread_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
    add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|--  Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
    )
endif()

add_executable (traffic_light "traffic_light.cpp")
target_link_libraries(traffic_light sfml-graphics sfml-window)

find_package (Threads)
find_package(SFML 2.5 COMPONENTS window system graphics REQUIRED)

link_libraries (Threads::Threads)
