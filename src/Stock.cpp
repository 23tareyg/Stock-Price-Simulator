#include <Stock.hpp>
#include <ParameterEst.hpp>


Stock::Stock() : name("N/A"), price(-1), mu(0.0), sigma(0.0) {}

Stock::Stock(std::string name, double price, double mu, double sigma) : name(name), price(price), mu(mu), sigma(sigma) {}

Stock::Stock(const json& stock) {
    name = stock["name"];
    price = stock["price"];
    mu = stock["mu"];
    sigma = stock["sigma"];
}


void Stock::estimateParameters(std::vector<double>& prices) {
    ParameterEstimator estimator(prices);

    auto [new_mu, new_sig] = estimator.estimateMuSigma();
    mu = new_mu;
    sigma = new_sig;
}
