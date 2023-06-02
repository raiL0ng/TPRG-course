import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd
import sys, math

Filename = None
# Crit_code = None
Data = None
Exp_val = None
Dev = None


def get_data_from_file() -> list:
  try:
    f = open(Filename, 'r')
    data = list(map(int, f.read().split(',')))
  except:
    print_help()
    exit(0)
  else:
    return data


def print_help():
  print('help')


# def choose_mode() -> str:
#   if Crit_code == 'hi':
#     print('hi-square')
#   elif Crit_code == 'se':
#     print('series')
#   elif Crit_code == 'in':
#     print('intervals')
#   elif Crit_code == 'di':
#     print('disintegration')
#   elif Crit_code == 'mo':
#     print('monotomy')
#   elif Crit_code == 'co':
#     print('conflict')
  
#   return ''

def get_probability_of_slice(seq : list) -> dict:
  probs = {}
  for el in seq:
    if el not in probs:
      probs[el] = 0
    probs[el] += 1
  for k, val in probs.items():
    probs[k] = val / len(seq)
  return probs


def get_expected_value(probs : dict) -> int:
  mu = 0
  for el, p in probs.items():
    mu += el * p
  return mu


def get_exp_val_and_dev() -> None:
  global Exp_val, Dev
  n = len(Data)
  step = (n // 100, 1)[n <= 100]
  l = n // step
  Exp_val = []
  posr = step
  for _ in range(l):
    Exp_val.append(get_expected_value(get_probability_of_slice(Data[0:posr])))
    posr += step
  Dev = []
  posr = step
  for l in range(len(Exp_val)):
    s = 0
    for el in Data[0:posr]:
      s += (el - Exp_val[l]) * (el - Exp_val[l])
    posr += step
    Dev.append(math.sqrt(s / step))
  return


def choose_mode() -> None:
  while True:
    print('\n1. Построить график зависимости математического ожидания от объема выборки')
    print('2. Построить график зависимости среднеквадратического отклонения от объема выборки')
    print('3. Вывести информацию о текущей ППСЧ')
    print('4. Выход')
    bl = input('Выберите опцию: ')
    if bl == '1':
      xs = [i for i in range(len(Exp_val))]
      xd = Exp_val[len(Exp_val) // 2]
      rel_err = round(abs(xd - Exp_val[-1]) / xd * 100, 2)
      df = pd.DataFrame({'Объём выборки': xs, 'Математическое ожидание': Exp_val})
      plt.figure(figsize=(10, 6), constrained_layout=True)
      t = sns.scatterplot(data=df, x='Объём выборки', y='Математическое ожидание', color='red')
      t.set(xlabel='Относительная погрешность измерения мат. ожидания ' + 
                   f'для выборки из {len(Data)} элементов: {rel_err}%')
      plt.show()
    elif bl == '2':
      xs = [i for i in range(len(Dev))]
      xd = Dev[len(Dev) // 2]
      rel_err = round(abs(xd - Dev[-1]) / xd * 100, 2)
      df = pd.DataFrame({'Объём выборки': xs, 'Cреднеквадратичное отклонение': Dev})
      plt.figure(figsize=(10, 6), constrained_layout=True)
      t = sns.scatterplot(data=df, x='Объём выборки', y='Cреднеквадратичное отклонение', color='blue')
      t.set(xlabel='Относительная погрешность измерения средн. отклонения' 
                   f' для выборки из {len(Data)} элементов: {rel_err}%')
      plt.show()
    elif bl == '4':
      break
  return





if __name__ == '__main__':
  for el in sys.argv:
    if '/f:' in el:
      Filename = el[3:]
      Data = get_data_from_file()
      mx = max(Data) + 1
      Data = [i / mx for i in Data]
    elif '/h' in el:
      print_help()
  if Filename == None or Data == None:
    print_help()
  else:
    get_exp_val_and_dev()
    choose_mode()
