cmake_minimum_required(VERSION 3.10)
project(StockPriceSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3 
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

add_executable(StockPriceSimulator 
    src/main.cpp
    src/Model.cpp
    src/Stock.cpp
)

target_include_directories(StockPriceSimulator PRIVATE include)

target_link_libraries(StockPriceSimulator PRIVATE 
    nlohmann_json::nlohmann_json 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)
