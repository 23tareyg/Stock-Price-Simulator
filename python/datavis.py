import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import glob
import os
import argparse
from datetime import datetime

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DEFAULT_GBM_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "output", "GBM"))
DEFAULT_ABM_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", "output", "ABM"))


def read_simulation_csv(path):
	"""Read a CSV and return a single pandas Series with a datetime or index-based index.

	Strategy: try parsing the first column as dates and use it as the index. If that fails,
	use the first column as the index without parsing. Then pick the first numeric column
	as the price series.
	"""
	try:
		df = pd.read_csv(path, parse_dates=[0], index_col=0)
	except Exception:
		df = pd.read_csv(path)
		try:
			df.set_index(df.columns[0], inplace=True)
		except Exception:
			pass

	num_cols = df.select_dtypes(include="number").columns.tolist()
	if not num_cols:
		raise ValueError(f"No numeric columns found in {path}")

	series = df[num_cols[0]].astype(float)
	series.name = os.path.splitext(os.path.basename(path))[0]
	return series


def plot_simulations(directory=DEFAULT_GBM_DIR, outpath=None):
    """Load all CSVs from `directory`, plot each simulation and the ensemble mean.

    Saves a PNG to `outpath` (defaults to `directory/gbm_simulations.png`) and shows the plot.
    """
    files = sorted(glob.glob(os.path.join(directory, "*.csv")))
    if not files:
        print("No CSV files found in", directory)
        return

    series_list = []
    for f in files:
        try:
            s = read_simulation_csv(f)
            series_list.append(s)
        except Exception as e:
            print("Skipping", f, "-", e)

    if not series_list:
        print("No valid series to plot.")
        return

    df = pd.concat(series_list, axis=1)
    mean_series = df.mean(axis=1)

    # plt.style.use("seaborn-darkgrid")
    plt.figure(figsize=(10, 6))

    for col in df.columns:
        plt.plot(df.index, df[col], color="C0", alpha=0.25, linewidth=0.8)

    plt.plot(mean_series.index, mean_series.values, color="C1", linewidth=2.0, label="Mean")

    plt.title("GBM Monte Carlo simulations")
    plt.xlabel("Time")
    plt.ylabel("Price")
    plt.legend()
    plt.tight_layout()

    if outpath is None:
        outpath = os.path.join(directory, "gbm_simulations.png")

    plt.savefig(outpath, dpi=150)
    print("Saved plot to", outpath)
    plt.show()


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Plot GBM simulation CSVs in a single chart")
    parser.add_argument("--dir", default=DEFAULT_GBM_DIR, help="directory with CSV files")
    parser.add_argument("--out", help="output image path")
    args = parser.parse_args()

    plot_simulations(args.dir, args.out)

