#include <Stock.hpp>


Stock::Stock() : name("N/A"), price(0.0), mu(0.0), sigma(0.0) {}

Stock::Stock(const json& stock) {
    name = stock["name"];
    price = stock["price"];
    mu = stock["mu"];
    sigma = stock["sigma"];
}
