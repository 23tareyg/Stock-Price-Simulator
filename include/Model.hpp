#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include "Stock.hpp"

using StockTime = std::chrono::time_point<std::chrono::system_clock>;

class PriceModel {
protected:
    std::vector<std::pair<StockTime, double>> stockData;
public:
    virtual ~PriceModel();
};

/*
usage:
i initialize a GBMModel with a stock, a duration, and a timestep, or perhaps neither
I can select some different durations, stocks, and timesteps from the dropdown
I click "run" and it simulates the stock from the model, storing the value and the datetime in a vector<pair<date, double>>
Simulation calculates formula and then stores the date and the double in the vector pair
Updates the date by incrementing it by timestep, and updates the stock.price by setting it equal to whatever the formula just outputted, and updates duration/timestep
repeats while duration/timestep > 0
*/
class GBMModel : PriceModel{
private:
    std::shared_ptr<Stock> stock;
    int duration_minutes;
    int timestep_minutes;

    StockTime currTime;
public:
    GBMModel(std::shared_ptr<Stock> s, int dur_min, int time_min);
    virtual void simulate(std::mt19937& mt);
    void print_data();
    virtual ~GBMModel();
};