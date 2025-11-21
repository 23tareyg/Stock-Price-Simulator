#include "ParameterEst.hpp"

double ParameterEstimator::computeMean(const std::vector<double>& data) const {
    if (data.empty()) throw std::invalid_argument("Data cannot be empty");
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double ParameterEstimator::computeStdDev(const std::vector<double>& data, double mean) const {
    if (data.size() < 2) throw std::invalid_argument("Data must have two or more members");

    double squareDiff = 0.0;
    for (double i : data) {
        squareDiff += (i - mean) * (i - mean);
    }
    return std::sqrt(squareDiff / (data.size() - 1));
}

std::pair<double, double> ParameterEstimator::estimateMuSigma() {
    // compute log returns
    std::vector<double> log_returns;
    for (int i = 1; i < prices.size(); i++) {
        if (prices[i] <= 0 || prices[i-1] <= 0) throw std::invalid_argument("Prices cannot be below zero");
        log_returns.push_back(std::log(prices[i] / prices[i-1]));
    }

    double mean_return = computeMean(log_returns);
    double std_dev = computeStdDev(log_returns, mean_return);

    double mu = mean_return * trading_days;
    double sigma = std_dev * std::sqrt(static_cast<double>(trading_days));
    return {mu, sigma};
}

