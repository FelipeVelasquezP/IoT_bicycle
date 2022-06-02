import pandas as pd


read_file = pd.read_csv ('temperaturas.txt')
read_file.to_csv ('temperaturas.csv', index=None)
