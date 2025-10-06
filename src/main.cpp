#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Stock.hpp"
#include "Model.hpp"

using json = nlohmann::json;

int main() {
    // parsing json data into data object
    std::ifstream f("../data/test.json");
    json data = json::parse(f);

    std::mt19937 prng;
    std::shared_ptr<Stock> test_stock = std::make_shared<Stock>(data["stocks"][0]);
    int duration = static_cast<int>(data["duration_minutes"]);
    int timestep = static_cast<int>(data["timestep_minutes"]);

    GBMModel model(test_stock, duration, timestep);
    model.simulate(prng);


    return 0;
}