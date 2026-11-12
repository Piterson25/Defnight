include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(tiny_aes
    GIT_REPOSITORY https://github.com/kokke/tiny-AES-c.git
    GIT_TAG master
)
FetchContent_MakeAvailable(tiny_aes)

