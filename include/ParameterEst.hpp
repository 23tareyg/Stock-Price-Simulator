#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <numeric>

class ParameterEstimator {
private:
    std::vector<double> prices;
    int trading_days = 252; // default

    double computeMean(const std::vector<double>& data) const;
    double computeStdDev(const std::vector<double>& data, double mean) const;

public:
    ParameterEstimator(const std::vector<double>& historicalPrices, int trading_days = 252) : prices(historicalPrices), trading_days(trading_days) {
        if (prices.empty()) throw std::invalid_argument("Prices cannot be empty");
    }
    std::pair<double, double> estimateMuSigma();

};


/*
need to take in a vector of prices
return a mu and sigma
mu is the average rate of return (often annualized)
sigma is the std deviation of returns

log returns r = ln(P_i/ P_i-1)

*/