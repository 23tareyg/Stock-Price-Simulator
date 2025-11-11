#include <iostream>
#include <vector>
#include <fstream>

#include <nlohmann/json.hpp>


#include "Stock.hpp"
#include "Model.hpp"
#include "ParameterEst.hpp"

using json = nlohmann::json;

int main() {
    // parsing json data into data object
    std::ifstream f("../data/test.json");
    json data = json::parse(f);

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 prng(seed1);
    std::shared_ptr<Stock> test_stock = std::make_shared<Stock>(data["stocks"][0]);
    int duration = static_cast<int>(data["duration_minutes"]);
    int timestep = static_cast<int>(data["timestep_minutes"]);
    TimeUnit t = TimeUnit::MINUTES;

    GBMModel model1(test_stock, duration, timestep, t);
    model1.simulate(prng);
    model1.print_data();
    
    ABMModel model2(test_stock, duration, timestep, t);
    model2.simulate(prng);
    model2.print_data();

/*
    std::vector<double> hist_prices = {100.0, 100.01, 100.03, 100.02};  // Example data
    ParameterEstimator estimator(hist_prices);
    std::pair<double, double> res = estimator.estimateMuSigma();
    double mu = res.first;
    double sigma = res.second;
    std::cout << mu << ", " << sigma << '\n';
*/
    return 0;
}