import matplotlib.pyplot as plt
# import matplotlib.gridspec as gridspec
import numpy as np
import sys, math

Filename = None
Crit_code = None
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


def choose_mode() -> str:
  if Crit_code == 'hi':
    print('hi-square')
  elif Crit_code == 'se':
    print('series')
  elif Crit_code == 'in':
    print('intervals')
  elif Crit_code == 'di':
    print('disintegration')
  elif Crit_code == 'mo':
    print('monotomy')
  elif Crit_code == 'co':
    print('conflict')
  
  return ''

def get_exp_val_and_st_dev() -> None:
  global Exp_val, Dev
  s = sum(Data)
  Exp_val = s / len(Data)
  print(f'\nExpected value of this sequence is {round(Exp_val, 3)}\n')
  s = 0
  for el in Data:
    s += (el - Exp_val) * (el - Exp_val)
  Dev = math.sqrt(s / len(Data))
  print(f'\nStandard deviation of this sequence is {round(Dev, 3)}\n')
  print(f'\nRelative error of this sequence is {round((Dev / Exp_val) * 100)}%\n')


if __name__ == '__main__':
  for el in sys.argv:
    if '/f:' in el:
      Filename = el[3:]
      Data = get_data_from_file()
    elif '/c:' in el:
      Crit_code = el[3:]
    elif '/h' in el:
      print_help()
  if Filename == None or Crit_code == None or Data == None:
    print_help()
  else:
    get_exp_val_and_st_dev()
