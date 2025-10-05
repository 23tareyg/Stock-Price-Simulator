#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Stock.hpp"
#include "Model.hpp"

using json = nlohmann::json;

int main() {
    std::ifstream f("../data/test.json");
    json data = json::parse(f);

    std::vector<Stock> * stocks = new std::vector<Stock>;

    for (const auto& stock : data["stocks"]) {
        Stock s(stock);
        stocks->push_back(s);
    }

    std::cout << stocks->at(0).name;
}