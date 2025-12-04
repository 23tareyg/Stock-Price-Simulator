#!/usr/bin/env python3
import os
import sys
import json
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Ensure we can import the compiled module in build/
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "build"))
if BUILD_DIR not in sys.path:
    sys.path.insert(0, BUILD_DIR)

try:
    import pystock
except Exception as e:
    raise ImportError(f"Could not import pystock from {BUILD_DIR}: {e}")


duration = 100
timestep = 1

# Map TimeUnit: 0=SECONDS,1=MINUTES,2=HOURS,3=DAYS (matches C++ enum order)
TIMEUNIT_HOURS = 2
NUM_ITER = 1000

print(f"Running {NUM_ITER} simulations (model=GBM) with duration={duration}, timestep={timestep}...")

test_stock = pystock.Stock("AAPL", 150, 0.1, 0.2)
arr = pystock.run_simulations("GBM", test_stock, duration, timestep, TIMEUNIT_HOURS, NUM_ITER)
# arr should be shape (NUM_ITER, steps)

arr = np.array(arr)

# Plot
sns.set_theme(style="darkgrid")
plt.figure(figsize=(12, 7))
steps = arr.shape[1]
xs = np.arange(steps)

# plot each simulation faintly
for i in range(arr.shape[0]):
    plt.plot(xs, arr[i, :], color="C0", alpha=0.08, linewidth=0.6)

# plot mean
mean_series = arr.mean(axis=0)
plt.plot(xs, mean_series, color="C1", linewidth=2.0, label="Mean")

plt.title(f"GBM Monte Carlo ({NUM_ITER} runs)")
plt.xlabel("Timestep")
plt.ylabel("Price")
plt.legend()
plt.tight_layout()

OUT_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "output", "GBM"))
os.makedirs(OUT_DIR, exist_ok=True)
outpath = os.path.join(OUT_DIR, f"pystock_gbm_{NUM_ITER}.png")
plt.savefig(outpath, dpi=150)
print("Saved plot to", outpath)
plt.show()
