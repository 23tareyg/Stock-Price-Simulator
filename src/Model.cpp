#include "Model.hpp"

PriceModel::~PriceModel() = default;

GBMModel::GBMModel(std::shared_ptr<Stock> s, int dur_min, int time_min)
    : stock(s), duration_minutes(dur_min), timestep_minutes(time_min)
{
    currTime = std::chrono::system_clock::now();
}

void GBMModel::simulate(std::mt19937 mt) {
    std::normal_distribution<double> dist{0.0, 1.0};
    double Z = dist(mt);
    int iteration = static_cast<int>(duration_minutes / timestep_minutes);
    double dt = timestep_minutes / (252 * 6.5 * 60); // normalizing dt for number of trading minutes there are in a year

    for (int i = 0; i < iteration; i++) {
        stock->price = stock->price * std::exp(stock->mu - (0.5 * stock->sigma * stock->sigma) * dt + stock->sigma * std::sqrt(dt) * Z);
        currTime += std::chrono::minutes(static_cast<int>(timestep_minutes));
        stockData.push_back({currTime, stock->price});
    }
}

void GBMModel::print_data() {
    for (auto& i : stockData) {
        std::time_t time = std::chrono::system_clock::to_time_t(i.first);
        std::tm* tm_ptr = std::localtime(&time);
        std::cout << "At " << std::put_time(tm_ptr, "%H:%M") << " the price was " << i.second << '\n';
    }
}

GBMModel::~GBMModel() = default;
