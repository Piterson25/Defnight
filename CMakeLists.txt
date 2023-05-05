cmake_minimum_required(VERSION 3.0.0)
project(Defnight VERSION 0.2.4)

set (CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "external/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "external/SFML-2.5.1/lib")

include_directories(${SFML_INCLUDE_DIR})

find_library(SFML_GRAPHICS_LIBRARY NAMES sfml-graphics PATHS ${SFML_LIBRARY_DIR})
find_library(SFML_WINDOW_LIBRARY NAMES sfml-window PATHS ${SFML_LIBRARY_DIR})
find_library(SFML_SYSTEM_LIBRARY NAMES sfml-system PATHS ${SFML_LIBRARY_DIR})
find_library(SFML_NETWORK_LIBRARY NAMES sfml-network PATHS ${SFML_LIBRARY_DIR})
find_library(SFML_AUDIO_LIBRARY NAMES sfml-audio PATHS ${SFML_LIBRARY_DIR})

file(GLOB SOURCES "src/*.cpp")

add_executable(Defnight ${SOURCES})
target_link_libraries(Defnight
    ${SFML_GRAPHICS_LIBRARY}
    ${SFML_WINDOW_LIBRARY}
    ${SFML_SYSTEM_LIBRARY}
    ${SFML_NETWORK_LIBRARY}
    ${SFML_AUDIO_LIBRARY}
)
