#include <iostream>
#include <memory>
#include <cassert>

#include "Stock.hpp"
#include "MonteCarlo.hpp"

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

    auto a = MC_Sim("GBM", stock, duration, timestep, timeUnitInt, iterations, seed);
    auto b = MC_Sim_threaded("GBM", stock, duration, timestep, timeUnitInt, iterations, seed);

    if (a.size() != b.size()) {
        std::cerr << "Size mismatch: single=" << a.size() << " threaded=" << b.size() << "\n";
        return 2;
    }

    size_t mismatches = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) ++mismatches;
    }

    if (mismatches == 0) {
        std::cout << "PASS: threaded output matches single-threaded output (seeded)" << std::endl;
        return 0;
    } else {
        std::cout << "FAIL: " << mismatches << " mismatches out of " << a.size() << std::endl;
        return 3;
    }
}
