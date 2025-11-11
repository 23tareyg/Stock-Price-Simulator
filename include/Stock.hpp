#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Stock {
public:
    std::string name;
    double price;
    double mu;
    double sigma;

public:
    Stock();
    Stock(std::ifstream& f);
    Stock(const json& input);
    void estimateParameters(std::vector<double>& prices);
};