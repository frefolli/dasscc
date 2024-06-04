"""
Plotting Unit
"""
import matplotlib.pyplot
import pesto.generics
import pesto.logs
import numpy
import os

class BasicPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.data: dict
    self.label: str
    self.x: str
    self.y: str
    Xs = numpy.array(self.data[self.x])
    Ys = numpy.array(self.data[self.y])
    matplotlib.pyplot.plot(Xs, Ys, marker='o', label=self.label)

class AggregatePlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.data: dict
    self.title: str
    self.x: str
    self.y: str
    fig = matplotlib.pyplot.figure(figsize=(15, 15))
    matplotlib.pyplot.title(self.title)
    for label in self.data:
      BasicPlotter.run(data=self.data[label], label=label, x=self.x, y=self.y)
    matplotlib.pyplot.xlabel(self.x)
    matplotlib.pyplot.ylabel(self.y)
    matplotlib.pyplot.legend()
    matplotlib.pyplot.savefig('./plots/logs/%s.png' % self.title)
    matplotlib.pyplot.close(fig)

class FunctionalPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.log: pesto.logs.DassLog
    self.crafter: pesto.generics.ServiceObject[dict]
    self.x: str
    self.y: str
    os.makedirs('./plots/logs/', exist_ok=True)
    data = self.crafter.run(log=self.log)
    for matrix in data:
      AggregatePlotter.run(data=data[matrix], title=matrix, x=self.x, y=self.y)
