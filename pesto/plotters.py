"""
Plotting Unit
"""
import matplotlib.pyplot
import pesto.generics
import pesto.logs
import logging
import numpy
import os

class BasicPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.data: dict
    self.label: str
    self.x: str
    self.y: str
    self.scale: str
    Xs = numpy.array(self.data[self.x])
    Ys = numpy.array(self.data[self.y])
    match self.scale:
      case 'linlin':
        matplotlib.pyplot.plot(Xs, Ys, marker='o', label=self.label)
      case 'linlog':
        matplotlib.pyplot.semilogy(Xs, Ys, marker='o', label=self.label)
      case 'loglin':
        matplotlib.pyplot.semilogx(Xs, Ys, marker='o', label=self.label)
      case 'loglog':
        matplotlib.pyplot.loglog(Xs, Ys, marker='o', label=self.label)

class AggregatePlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.data: dict
    self.title: str
    self.x: str
    self.y: str
    self.scale: str
    self.outdir: str
    filepath = os.path.join(self.outdir, '%s.png' % self.title)
    logging.info("Generating %s" % filepath)
    fig = matplotlib.pyplot.figure(figsize=(15, 15))
    matplotlib.pyplot.title(self.title)
    for label in self.data:
      BasicPlotter.run(data=self.data[label], label=label, x=self.x, y=self.y, scale=self.scale)
    matplotlib.pyplot.xlabel(self.x)
    matplotlib.pyplot.ylabel(self.y)
    matplotlib.pyplot.legend()
    matplotlib.pyplot.savefig(filepath)
    matplotlib.pyplot.close(fig)
    logging.info("Done with %s" % filepath)

class FunctionalPlotter(pesto.generics.ServiceObject[None]):
  def exec(self) -> None:
    self.log: pesto.logs.DassLog
    self.crafter: pesto.generics.ServiceObject[dict]
    self.x: str
    self.y: str
    self.scale: str
    self.outdir: str
    os.makedirs(self.outdir, exist_ok=True)
    data = self.crafter.run(log=self.log)
    for group in data:
      AggregatePlotter.run(data=data[group], title=group, x=self.x, y=self.y, scale=self.scale, outdir=self.outdir)
