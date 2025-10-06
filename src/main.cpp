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

    std::vector<Stock> * stocks = new std::vector<Stock>; // pointer to vector of Stock objects
    std::mt19937 prng;
    double timestep = data["timestep_minutes"];
    double duration = data["duration_minutes"];
    double min_per_year = 252*6.5*60;
    double dt = timestep / min_per_year;

    for (const auto& stock : data["stocks"]) {
        Stock s(stock);
        stocks->push_back(s);
    }

    std::vector<std::vector<double>> res; // result vector
    int num_steps = duration / timestep;
    res.resize(stocks->size(), std::vector<double>(num_steps));

    for (int i = 0; i < (*stocks).size(); i++) {
        int temp = duration;
        GBMModel model;
        int step = 0;
        while (temp > 0) {
            double calc = model.step(stocks->at(i).price, stocks->at(i).mu, stocks->at(i).sigma, dt, prng);
            res[i][step] = calc;
            stocks->at(i).price = calc;
            step++;
            temp -= timestep;
        }
    }

    for(auto i : res) {
        std::cout << "Stock 1:\n";
        for (auto j : i) {
            std::cout << j << ',' << '\n';
        }
    }

    delete stocks;

    return 0;
}