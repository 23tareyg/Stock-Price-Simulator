#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>
#include "Model.hpp"
#include "Stock.hpp"
std::vector<double> MC_Sim(const std::string &modelType, std::shared_ptr<Stock> stock,
    int duration, int timestep, int timeUnitInt, int num_iterations, unsigned seed = 0) {

    // create a prototype model to determine number of steps
    TimeUnit unit = static_cast<TimeUnit>(timeUnitInt);
    auto proto = PriceModel::createModel(modelType, stock, duration, timestep, unit);
    int steps = static_cast<int>(duration / timestep) + 1; // include initial

    //result array preallocated
    std::vector<double> result((size_t)num_iterations * steps);

    unsigned seed1 = seed ? seed : std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 orig_prng(seed1);

    for (int i = 0; i < num_iterations; i++) {
        // copy prng and advance so runs differ
        auto prng = orig_prng;
        prng.discard(1000 + i);

        auto model = PriceModel::createModel(modelType, stock, duration, timestep, unit);
        model->simulate(prng);

        // collect prices from model and write into preallocated buffer
        auto prices = model->getPriceSeries();
        size_t offset = (size_t)i * steps;
        for (int j = 0; j < steps; ++j) {
            result[offset + j] = prices[j];
        }
    }

    return result;  
}

std::vector<double> MC_Sim_threaded(const std::string &modelType, std::shared_ptr<Stock> stock,
    int duration, int timestep, int timeUnitInt, int num_iterations, unsigned seed = 0) {
    /*
    check bounds on num iterations
    set seed if seed does not exist
    check for number of hardware threads using thread::hardware_concurrency()
    create worker lambda whcih runs a model and simulates it
    loop through each thread and emplace_back a worker
    loop through each thread object and join if they are joinable (done with task)
    return result
    */

    if (num_iterations < 10) return {};

    // create a prototype model to determine number of steps
    TimeUnit unit = static_cast<TimeUnit>(timeUnitInt);
    auto proto = PriceModel::createModel(modelType, stock, duration, timestep, unit);
    int steps = static_cast<int>(duration / timestep) + 1; // include initial

    // result array preallocated and sized so worker threads can write into disjoint ranges
    std::vector<double> result((size_t)num_iterations * steps);

    unsigned seed1 = seed ? seed : std::chrono::system_clock::now().time_since_epoch().count();

    unsigned hwthreads = std::thread::hardware_concurrency();
    unsigned num_threads = std::max(1u, hwthreads);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    auto worker = [&](int start, int end) {
        std::mt19937 original(seed1);

        for (int i = start; i < end; i++) {
            auto prng = original;
            prng.discard(1000 + i);

            auto model = PriceModel::createModel(modelType, stock, duration, timestep, unit);
            model->simulate(prng);
            auto prices = model->getPriceSeries();

            size_t offset = (size_t)i * steps; //calc offset
            for (int j = 0; j < steps; j++) {
                result[offset + j] = prices[j];
            }
        }
    };

    int per = num_iterations / num_threads; //floor
    int rem = num_iterations % num_threads;
    int curr = 0;

    for (int i = 0; i < num_threads; i++) {
        // evenly split start and end indices so threads have even work
        int start = curr;
        int count = per + (rem > i ? 1 : 0);
        int end = start + count;
        threads.emplace_back(worker, start, end);
        curr = end;
    }

    for (auto& th : threads) if (th.joinable()) th.join();

    return result;
}