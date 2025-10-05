#pragma once
#include <random>
#include <cmath>
#include "Stock.hpp"

class PriceModel {
public:
    virtual double step(const double price, const double mu, const double sigma, double dt, std::mt19937& mt);
    virtual ~PriceModel();
};

class GBMModel : PriceModel{
public:
    double step(const double price, const double mu, const double sigma, double dt, std::mt19937& mt);
    virtual ~GBMModel();
};