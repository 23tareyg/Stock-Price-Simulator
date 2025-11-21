#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Stock object is initializd with a mu and a sigma which should be caluclated
// elsewhere in the program or chosen based on historical values
class Stock {
public:
    std::string name;
    double price;
    double mu;
    double sigma;

public:
    Stock();
    Stock(std::string name, double price, double mu, double sigma);
    Stock(std::ifstream& f);
    Stock(const json& input);
    void estimateParameters(std::vector<double>& prices);
};