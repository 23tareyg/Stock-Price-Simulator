#include <iostream>
#include <memory>
#include <cassert>

#include "Stock.hpp"
#include "MonteCarlo.hpp"

using StockTime = std::chrono::time_point<std::chrono::system_clock>;

int main() {
    // construct a stock
    nlohmann::json j;
    j["name"] = "TEST";
    j["price"] = 100.0;
    j["mu"] = 0.05;
    j["sigma"] = 0.2;

    auto stock = std::make_shared<Stock>(j);
    int duration = 100;
    int timestep = 1;
    int timeUnitInt = static_cast<int>(TimeUnit::HOURS);
    int iterations = 100;
    unsigned seed = 42u;

    for (int i = iterations; i <= 100'000; i*=10) {
    {
        auto start = std::chrono::steady_clock::now();
        auto result = MC_Sim("GBM", stock, duration, timestep, timeUnitInt, i, seed);
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> exec_time = end - start;
        std::cout << "MC Sim for " << i << " iterations is " << exec_time.count() << " seconds. "
            << "(returned elements): " << result.size() << '\n';
    }
    {
        auto start = std::chrono::steady_clock::now();
        auto result = MC_Sim_threaded("GBM", stock, duration, timestep, timeUnitInt, i, seed);
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> exec_time = end - start;
        std::cout << "MC Sim threaded for " << i << " iterations is " << exec_time.count() << " seconds. "
            << "(returned elements): " << result.size() << '\n';
    }
    std::cout << "\n\n";
    }
}

