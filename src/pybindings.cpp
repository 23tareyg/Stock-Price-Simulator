#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "Model.hpp"
#include "Stock.hpp"

#include <nlohmann/json.hpp>
#include <random>

namespace py = pybind11;
using json = nlohmann::json;

// run multiple sims and return 2d numpy array
py::array_t<double> run_simulations(const std::string &modelType, std::shared_ptr<Stock> stock,
        int duration, int timestep, int timeUnitInt, int num_iterations) {

    // create a prototype model to determine number of steps
    TimeUnit unit = static_cast<TimeUnit>(timeUnitInt);
    auto proto = PriceModel::createModel(modelType, stock, duration, timestep, unit);
    int steps = static_cast<int>(duration / timestep) + 1; // include initial

    // result array can be preallocated
    std::vector<double> result;
    result.reserve((size_t)num_iterations * steps);

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 orig_prng(seed1);

    for (int i = 0; i < num_iterations; i++) {
        // copy prng and advance so runs differ
        auto prng = orig_prng;
        prng.discard(1000 + i);

        auto model = PriceModel::createModel(modelType, stock, duration, timestep, unit);
        model->simulate(prng);

        // collect prices from model
        auto prices = model->getPriceSeries();
        for (double v : prices) result.push_back(v);
    }

    // create 2d numpy array (num_iterations, steps)
    py::array_t<double> arr({(size_t)num_iterations, (size_t)steps});
    auto buf = arr.mutable_unchecked<2>();

    for (int i = 0; i < num_iterations; ++i) {
        for (int j = 0; j < steps; ++j) {
            buf(i, j) = result[(size_t)i * steps + j];
        }
    }

    return arr;
}

// duplicate of PriceModel create model method as to not import the entire class
std::unique_ptr<PriceModel> create_model(const std::string& type, std::shared_ptr<Stock> stock, int dur, int t, TimeUnit unit) {
    if (type == "GBM") return std::make_unique<GBMModel>(stock, dur, t, unit);
    if (type == "ABM") return std::make_unique<ABMModel>(stock, dur, t, unit);
    throw std::invalid_argument("Invalid model type");
}

PYBIND11_MODULE(pystock, m) {
    m.doc() = "pybind11 bindings for StockPriceSimulator";

    py::class_<Stock, std::shared_ptr<Stock>>(m, "Stock")
        .def(py::init([](const std::string name, double price, double mu, double sigma) {
            return std::make_shared<Stock>(name, price, mu, sigma);
        }), py::arg("name"), py::arg("price"), py::arg("mu"), py::arg("sigma"));

    m.def("run_simulations", &run_simulations, "Run Monte Carlo simulations and return a NumPy array",
        py::arg("modelType"), py::arg("stock_dict"), py::arg("duration"), py::arg("timestep"), 
        py::arg("timeUnitInt"), py::arg("num_iterations"));
    
    m.def("create_model", &create_model, "Create a stock prediction model",
        py::arg("type"), py::arg("stock"), py::arg("dur"), py::arg("t"), py::arg("unit"));
}
