"""
Plotting Unit
"""
import numpy
import matplotlib.pyplot as plt
import matplotlib.axes as axes
import json

def load_report(path: str) -> dict:
  data = {}
  with open(path, mode="rb") as file:
    _data = json.load(file)
    for key in _data:
      data[key] = {}
      for cell in _data[key]:
        cell["nonZeros"] = cell['N'] * cell['M'] * cell['density']
      _data[key] = sorted(_data[key], key=lambda cell: cell["nonZeros"])
      for cell in _data[key]:
        for label in cell:
          if label not in data[key]:
            data[key][label] = []
          data[key][label].append(cell[label])
  return data

def get_xy(report: dict, actuator: str) -> tuple:
  data = report.get(actuator)
  if data is not None:
    return numpy.array(data["nonZeros"]), numpy.array(data['elapsed'])
  raise ValueError('unable to find actuator %s' % actuator)

def plot_by_mode(mode: str, *args, **kargs):
  match mode:
    case 'LinLog':
      plt.semilogy(*args, **kargs)
    case 'LinLin':
      plt.plot(*args, **kargs)
    case _:
      raise ValueError('unknown mode \'%s\'' % mode)

def plot_report(report: dict, mode: str = 'LinLog'):
  path = 'benchmark-%s.png' % mode
  fig = plt.figure(figsize=(15, 15))
  for actuator in report:
    Xs, Ys = get_xy(report, actuator)
    plot_by_mode(mode, Xs, Ys, label=actuator, marker='o')
  plt.xlabel('nonZeros')
  plt.ylabel('elapsed')
  plt.legend(fancybox=True, shadow=True)
  plt.title('Benchmark C++ (%s)' % mode)
  plt.savefig(path)
  plt.close(fig)

if __name__ == "__main__":
  report = load_report('report.json')
  plot_report(report, mode='LinLog')
  plot_report(report, mode='LinLin')
