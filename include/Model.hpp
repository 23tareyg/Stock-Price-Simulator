#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <stdexcept>
#include <memory>

#include "Stock.hpp"

using StockTime = std::chrono::time_point<std::chrono::system_clock>;

// Time unit enum to handle changes in timestep
enum class TimeUnit {
    SECONDS,
    MINUTES,
    HOURS,
    DAYS
};

class PriceModel {
protected:
    std::vector<std::pair<StockTime, double>> stockData;
    StockTime curr_time;
public:
    // define a pure virtual "simulate" function by setting to zero to prevent runtime problems
    virtual void simulate(std::mt19937& mt) = 0;

    virtual void print_data();
    static std::unique_ptr<PriceModel> createModel(const std::string& type, std::shared_ptr<Stock> stock, int dur, int t, TimeUnit unit);
    virtual void reset();
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


class GBMModel : public PriceModel {
private:
    std::shared_ptr<Stock> stock;
    int duration;
    int timestep;
    TimeUnit unit;
    double normalize = 252; // standard value is days

public:
    GBMModel(std::shared_ptr<Stock> s, int dur_unit, int tstep, TimeUnit t);
    void simulate (std::mt19937& mt);
    void print_data();
    virtual ~GBMModel();
};

class ABMModel : public PriceModel{
private:
    std::shared_ptr<Stock> stock;
    int duration;
    int timestep;
    TimeUnit unit;
    double normalize;

public:
    ABMModel(std::shared_ptr<Stock> s, int dur_unit, int tstep, TimeUnit t);
    void simulate (std::mt19937& mt);
    void print_data();
    virtual ~ABMModel();
};