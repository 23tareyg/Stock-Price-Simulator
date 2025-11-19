<!--
Purpose: concise, actionable guidance for AI code assistants working on this repo.
Keep this file short (20-50 lines). Update when project layout or build changes.
-->

# Quick orientation for automated coding agents

This repository is a small C++ stock-price simulator (CMake + single executable). Below are the essential facts, conventions, and runnable examples an AI agent needs to be productive.

## Quick start (build & run)
- From project root: configure and build with CMake

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -j
# run the binary from the build directory because the executable expects `../data/test.json`
cd build && ./StockPriceSimulator
```

Note: a built binary has been observed in `build/StockPriceSimulator` (sometimes `build/bin/StockPriceSimulator` depending on generator).

## Big-picture architecture
- Single executable: `StockPriceSimulator` (sources under `src/`, headers under `include/`).
- Data input: `src/main.cpp` reads `../data/test.json` at runtime (so run the binary from `build/` or adapt the path).
- Core components:
  - `PriceModel` (abstract base) — defined in `include/Model.hpp`, implemented in `src/Model.cpp`.
    - Concrete models: `GBMModel` and `ABMModel` (both store `stockData` as `vector<pair<StockTime,double>>`).
  - `Stock` — simple value object in `include/Stock.hpp` / `src/Stock.cpp` (constructed from JSON).
  - `ParameterEstimator` — in `include/ParameterEst.hpp` / `src/ParameterEst.cpp` (estimates mu & sigma from historical prices).

## Project-specific conventions & patterns
- Headers live in `include/` and are added via `target_include_directories(StockPriceSimulator PRIVATE include)`.
- Dependency management: uses CMake `FetchContent` to bring in `nlohmann::json` (see `CMakeLists.txt`).
- Factory pattern: `PriceModel::createModel(type, stock, dur, t, unit)` expects `type` equal to the literal strings `"GBM"` or `"ABM"`.
- Time handling: `TimeUnit` enum in `Model.hpp` drives normalization and time increments. Each model sets `normalize` based on `TimeUnit`.

## Important examples (copyable snippets)
- Creating a model via factory (from existing code):
  - `auto m = PriceModel::createModel("GBM", stock_ptr, duration, timestep, TimeUnit::MINUTES);`
- `main.cpp` example: builds `GBMModel` and `ABMModel` and calls `simulate(prng)` then `print_data()`.

## Integration points & gotchas discovered in code
- Runtime input: `src/main.cpp` opens `../data/test.json` relative to the working directory. CI or tools must run the binary from `build/` or change the path.
- Randomness: seed is taken from `std::chrono::system_clock::now()` in `main.cpp`, so runs are non-deterministic unless you inject a seed.
- Parameter estimation: `ParameterEstimator::estimateMuSigma()` computes log-returns using `std::log(prices[i]) / std::log(prices[i-1])` — this looks like a likely bug (expected `std::log(prices[i] / prices[i-1])` or `std::log(prices[i]) - std::log(prices[i-1])`). Flag for reviewers.

## Where to make common edits
- Add new models: implement a class deriving from `PriceModel` (update `createModel` in `src/Model.cpp`).
- Add CLI or config: modify `src/main.cpp` to accept a path/seed/flags rather than hard-coded `../data/test.json` and PRNG seed.
- Tests: there are no unit tests. Prefer small, header-only test harnesses (Catch2) or simple `main`-based smoke tests until test infra is added.

## Key files (refer to these directly)
- `src/main.cpp` — app entry, data path, example simulation calls
- `include/Model.hpp` / `src/Model.cpp` — PriceModel, GBMModel, ABMModel, factory
- `include/Stock.hpp` / `src/Stock.cpp` — Stock JSON construction and parameter update
- `include/ParameterEst.hpp` / `src/ParameterEst.cpp` — estimator logic (watch the log-return calculation)
- `CMakeLists.txt` — build + FetchContent for `nlohmann::json`

If any section is unclear or you want me to expand examples (e.g., a small deterministic test harness or an example patch to accept a CLI seed/path), tell me which part to expand.
