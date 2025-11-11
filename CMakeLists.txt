cmake_minimum_required(VERSION 3.16)
project(StockPriceSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3 
)
FetchContent_MakeAvailable(json)

add_executable(StockPriceSimulator 
    src/main.cpp
    src/Model.cpp
    src/Stock.cpp
    src/ParameterEst.cpp
)

target_include_directories(StockPriceSimulator PRIVATE include)

target_link_libraries(StockPriceSimulator PRIVATE 
    nlohmann_json::nlohmann_json 
)
