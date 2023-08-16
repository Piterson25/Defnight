cmake_minimum_required(VERSION 3.16)
project(Defnight VERSION 0.2.4)

add_subdirectory(external/SFML)
add_subdirectory(external/cryptopp)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/Audio/*.cpp" "src/Components/*.cpp" "src/Effects/*.cpp"
    "src/Entities/*.cpp" "src/Entities/Bosses/*.cpp" "src/Entities/Drops/*.cpp" "src/Entities/Monsters/*.cpp"
    "src/Entities/Player/*.cpp" "src/Entities/Projectiles/*.cpp" "src/Game/*.cpp" "src/GUI/*.cpp"
    "src/Map/*.cpp" "src/States/*.cpp" "src/Systems/*.cpp" "src/Utils/*.cpp")

add_executable(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${CRYPTOPP_SOURCES})

set(INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_SOURCE_DIR}/src/Audio/"
    "${CMAKE_SOURCE_DIR}/src/Components/"
    "${CMAKE_SOURCE_DIR}/src/Effects/"
    "${CMAKE_SOURCE_DIR}/src/Entities/"
    "${CMAKE_SOURCE_DIR}/src/Entities/Bosses/"
    "${CMAKE_SOURCE_DIR}/src/Entities/Drop/"
    "${CMAKE_SOURCE_DIR}/src/Entities/Monsters/"
    "${CMAKE_SOURCE_DIR}/src/Entities/Player/"
    "${CMAKE_SOURCE_DIR}/src/Entities/Projectiles/"
    "${CMAKE_SOURCE_DIR}/src/Game/"
    "${CMAKE_SOURCE_DIR}/src/GUI/"
    "${CMAKE_SOURCE_DIR}/src/Map/"
    "${CMAKE_SOURCE_DIR}/src/States/"
    "${CMAKE_SOURCE_DIR}/src/Systems/"
    "${CMAKE_SOURCE_DIR}/src/Utils/")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio cryptopp)
