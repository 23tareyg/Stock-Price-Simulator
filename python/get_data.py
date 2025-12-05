import yfinance as yf
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# start and end format must be string "YYYY-MM-DD"
def get_data(ticker, period=None, start=None, end=None, interval="1d"):
    stock = yf.Ticker(ticker)
    
    if period:
        df = stock.history(period=period, interval=interval)
    else:
        df = stock.history(start=start, end=end, interval=interval)
    
    return df

def main():
    df = get_data("MDB", period="10mo")
    df.plot()
    plt.show()

if __name__ == "__main__":
    main()