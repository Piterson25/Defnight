cmake_minimum_required(VERSION 3.0.0)
project(Defnight VERSION 0.2.4)

set(CMAKE_CXX_STANDARD 23)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/external/SFML-2.5.1/include")

include_directories("${CMAKE_SOURCE_DIR}/include")

file(GLOB SOURCES "src/*.cpp")

add_executable(Defnight ${SOURCES})

target_link_libraries(Defnight sfml-graphics sfml-window sfml-system sfml-audio)

set_target_properties(Defnight PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release"
)
