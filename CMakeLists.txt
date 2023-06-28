cmake_minimum_required(VERSION 3.0.0)
project(Defnight VERSION 0.2.4)

set(CMAKE_CXX_STANDARD 23)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML-2.6.0/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/external/SFML-2.6.0/include")
include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/src/Audio")
include_directories("${CMAKE_SOURCE_DIR}/src/Components")
include_directories("${CMAKE_SOURCE_DIR}/src/Effects")
include_directories("${CMAKE_SOURCE_DIR}/src/Entities")
include_directories("${CMAKE_SOURCE_DIR}/src/Entities/Bosses")
include_directories("${CMAKE_SOURCE_DIR}/src/Entities/Drop")
include_directories("${CMAKE_SOURCE_DIR}/src/Entities/Monsters")
include_directories("${CMAKE_SOURCE_DIR}/src/Entities/Player")
include_directories("${CMAKE_SOURCE_DIR}/src/Entities/Projectiles")
include_directories("${CMAKE_SOURCE_DIR}/src/Game")
include_directories("${CMAKE_SOURCE_DIR}/src/GUI")
include_directories("${CMAKE_SOURCE_DIR}/src/Map")
include_directories("${CMAKE_SOURCE_DIR}/src/States")
include_directories("${CMAKE_SOURCE_DIR}/src/Systems")
include_directories("${CMAKE_SOURCE_DIR}/src/Utils")

file(GLOB SOURCES "src/*.cpp" "src/Audio/*.cpp" "src/Components/*.cpp" "src/Effects/*.cpp"
"src/Entities/*.cpp" "src/Entities/Bosses/*.cpp" "src/Entities/Drops/*.cpp" "src/Entities/Monsters/*.cpp"
"src/Entities/Player/*.cpp" "src/Entities/Projectiles/*.cpp" "src/Game/*.cpp" "src/GUI/*.cpp"
"src/Map/*.cpp" "src/States/*.cpp" "src/Systems/*.cpp" "src/Utils/*.cpp")

add_executable(Defnight ${SOURCES} )

target_link_libraries(Defnight sfml-graphics sfml-window sfml-system sfml-audio)

set_target_properties(Defnight PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release"
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET Defnight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_BINARY_DIR}/bin/debug/assets)

    add_custom_command(TARGET Defnight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/config
                ${CMAKE_BINARY_DIR}/bin/debug/config)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET Defnight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_BINARY_DIR}/bin/release/assets)

    add_custom_command(TARGET Defnight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/config
                ${CMAKE_BINARY_DIR}/bin/release/config)
endif()

