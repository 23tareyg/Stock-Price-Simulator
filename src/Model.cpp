#include "Model.hpp"
#include <ctime>
// Price Class
void PriceModel::print_data() {
    for (const auto& i : stockData) {
        std::time_t time = std::chrono::system_clock::to_time_t(i.first);
        std::tm* tm_ptr = std::localtime(&time);
        std::cout << "At " << std::put_time(tm_ptr, "%H:%M") << " the price will be " << i.second << '\n';
        //std::cout << "At " << std::ctime(&time) << " the price will be " << i.second << '\n';
    }
}

void PriceModel::reset() {
    stockData.clear();
    curr_time = std::chrono::system_clock::now();
}

void PriceModel::exportToCSV(std::string& filename) {
    std::ofstream file(filename);
    file << "time,price\n";
    for (const auto& [time, price] : stockData) {
        std::time_t time_t = std::chrono::system_clock::to_time_t(time);
        std::tm* tm = std::localtime(&time_t);
        file << std::put_time(tm, "%Y-%m-%d %H:%M:%S") << "," << price << '\n';
    }
}

std::unique_ptr<PriceModel> PriceModel::createModel(const std::string& type, std::shared_ptr<Stock> stock, int dur, int t, TimeUnit unit) {
    if (type == "GBM") return std::make_unique<GBMModel>(stock, dur, t, unit);
    if (type == "ABM") return std::make_unique<ABMModel>(stock, dur, t, unit);
    throw std::invalid_argument("Invalid model type");
}

std::vector<double> PriceModel::getPriceSeries() const {
    std::vector<double> out;
    out.reserve(stockData.size());
    for (const auto &p : stockData) out.push_back(p.second);
    return out;
}

PriceModel::~PriceModel() = default;



// GBM Class
GBMModel::GBMModel(std::shared_ptr<Stock> s, int dur_unit, int tstep, TimeUnit t) : 
    stock(std::move(s)), duration(dur_unit), timestep(tstep), unit(t) {
    // initialize normalize unit based on time step
    switch (t) {
        case (TimeUnit::SECONDS):
            normalize = (252.0 * 6.5 * 60.0 * 60.0);
            break;
        case (TimeUnit::MINUTES):
            normalize = (252.0 * 6.5 *60.0);
            break;
        case (TimeUnit::HOURS):
            normalize = (252.0 * 6.5);
            break;
        case (TimeUnit::DAYS):
            normalize = 252.0;
            break;
        default:
            break;
    }

    curr_time = std::chrono::system_clock::now();
}

void GBMModel::simulate(std::mt19937& mt) {
    std::normal_distribution<double> dist{0.0, 1.0};
    int iteration = static_cast<int>(duration / timestep);
    double dt = static_cast<double>(timestep) / normalize; // fraction of year
    double curr_price = stock->price;

    stockData.push_back({curr_time, curr_price});

    for (int i = 0; i < iteration; i++) {
        double Z = dist(mt);
        double drift = (stock->mu - 0.5 * stock->sigma * stock->sigma) * dt;
        double diffusion = stock->sigma * std::sqrt(dt) * Z;
        curr_price *= std::exp(drift + diffusion);

        switch(unit) {
            case TimeUnit::SECONDS:
                curr_time += std::chrono::seconds(timestep);
                break;
            case TimeUnit::MINUTES: 
                curr_time += std::chrono::minutes(timestep);
                break;
            case TimeUnit::HOURS:
                curr_time += std::chrono::hours(timestep);
                break;
            case TimeUnit::DAYS:
                curr_time += std::chrono::days(timestep);
                break;
        }

        stockData.push_back({curr_time, curr_price});
    }
}

void GBMModel::print_data() {
    std::cout << "GBM Model Data\n";
    for (const auto& i : stockData) {
        std::time_t time = std::chrono::system_clock::to_time_t(i.first);
        std::tm* tm_ptr = std::localtime(&time);
        std::cout << "At " << std::put_time(tm_ptr, "%H:%M") << " the price will be " << i.second << '\n';
        // std::cout << "At " << std::ctime(&time) << " the price will be " << i.second << '\n';
    }
}

GBMModel::~GBMModel() = default;

// ABM Class
ABMModel::ABMModel(std::shared_ptr<Stock> s, int dur_unit, int tstep, TimeUnit t) 
    : stock(std::move(s)), duration(dur_unit), timestep(tstep), unit(t) {
    switch (t) {
        case (TimeUnit::SECONDS):
            normalize = (252.0 * 6.5 * 60.0 * 60.0);
            break;
        case (TimeUnit::MINUTES):
            normalize = (252.0 * 6.5 * 60.0);
            break;
        case (TimeUnit::HOURS):
            normalize = (252.0 * 6.5);
            break;
        case (TimeUnit::DAYS):
            normalize = 252.0;
            break;
        default:
            break;
    }

    curr_time = std::chrono::system_clock::now();
}

void ABMModel::simulate(std::mt19937& mt) {
    std::normal_distribution<double> dist{0.0, 1.0};
    int iteration = static_cast<int>(duration / timestep);
    double dt = static_cast<double>(timestep) / normalize; // fraction of year
    double curr_price = stock->price;

    stockData.push_back({curr_time, curr_price});

    for (int i = 0; i < iteration; i++) {
        double Z = dist(mt);
        double drift = stock->mu * dt;
        double diffusion = stock->sigma * std::sqrt(dt) * Z;
        curr_price += drift + diffusion;

        if (curr_price < 0) {
            curr_price = 0.01; // min price of a stock
        }

        switch(unit) {
            case TimeUnit::SECONDS:
                curr_time += std::chrono::seconds(timestep);
                break;
            case TimeUnit::MINUTES: 
                curr_time += std::chrono::minutes(timestep);
                break;
            case TimeUnit::HOURS:
                curr_time += std::chrono::hours(timestep);
                break;
            case TimeUnit::DAYS:
                curr_time += std::chrono::days(timestep);
                break;
        }

        stockData.push_back({curr_time, curr_price});
    }
}

void ABMModel::print_data() {
    std::cout << "ABM Model Data\n";
    for (const auto& i : stockData) {
        std::time_t time = std::chrono::system_clock::to_time_t(i.first);
        std::tm* tm_ptr = std::localtime(&time);
        std::cout << "At " << std::put_time(tm_ptr, "%H:%M") << " the price will be " << i.second << '\n';
    }
}

ABMModel::~ABMModel() = default;