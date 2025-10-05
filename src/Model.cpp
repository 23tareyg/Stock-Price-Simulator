#include "Model.hpp"

double PriceModel::step(const double price, const double mu, const double sigma, double dt, std::mt19937& mt) {
    std::normal_distribution<double> dist{0.0, 1.0};
    double Z = dist(mt);
    double ans = price * std::exp(mu - (0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * Z);
    return ans;
}

PriceModel::~PriceModel() = default;

double GBMModel::step(const double price, const double mu, const double sigma, double dt, std::mt19937& mt) {
    std::normal_distribution<double> dist{0.0, 1.0};
    double Z = dist(mt);
    double ans = price * std::exp(mu - (0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * Z);
    return ans;
}

GBMModel::~GBMModel() = default;
