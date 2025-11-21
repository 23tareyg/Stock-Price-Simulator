cmake_minimum_required(VERSION 3.22)
project(StockPriceSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
set(MY_MODULE_SRCS pybindings.cpp)


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

pybind11_add_module(pystock
    src/pybindings.cpp
    src/Model.cpp
    src/Stock.cpp
    src/ParameterEst.cpp
)

target_include_directories(pystock PRIVATE include)
target_link_libraries(pystock PRIVATE nlohmann_json::nlohmann_json)
