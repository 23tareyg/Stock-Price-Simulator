#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>


#include "Stock.hpp"
#include "Model.hpp"
#include "ParameterEst.hpp"

using json = nlohmann::json;

// we're trying to run multiple simulations of a model with a certain timestep
// want to output to multiple files in file directory
void monteCarloSim(const std::string& modelType, std::shared_ptr<Stock> stock,
int duration, int timestep, TimeUnit t, const std::string& outputDir, int num_iterations) {

    std::filesystem::create_directory(outputDir);

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 orig_prng(seed1);

    for (int i = 1; i <= num_iterations; i++) {
        auto prng = orig_prng;
        prng.discard(1000); // advance engine by 1000

        auto model = PriceModel::createModel(modelType, stock, duration, timestep, t);
        model->simulate(prng);

        std::string filename = outputDir + "/" + modelType + std::to_string(i) + ".csv";
        model->exportToCSV(filename);
    }
}

int main() {

    // parsing json data into data object
    std::ifstream f("../data/test.json");
    json data = json::parse(f);

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 prng(seed1);
    std::shared_ptr<Stock> test_stock = std::make_shared<Stock>(data["stocks"][0]);
    int duration = static_cast<int>(data["duration_minutes"]);
    int timestep = static_cast<int>(data["timestep_minutes"]);
    TimeUnit t = TimeUnit::HOURS;

    // monteCarloSim("GBM", test_stock, duration, timestep, t, "/home/tarey/dev/Projects/Stock-Price-Simulator/output", 10);

    auto model = PriceModel::createModel("GBM", test_stock, duration, timestep, t);
    model->simulate(prng);
    model->print_data();

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