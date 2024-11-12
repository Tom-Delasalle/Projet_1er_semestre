cmake_minimum_required (VERSION 3.9)

project(test_plane)


set(SRCS "plane.cpp")

add_executable (test_plane ${SRCS})

find_package (Threads)

target_compile_features(test_plane PUBLIC cxx_std_17)
target_link_libraries (test_plane Threads::Threads)

install(TARGETS test_plane  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/../bin)
