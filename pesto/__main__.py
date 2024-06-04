import typing
import sys
import argparse
from pesto.loaders import DassLogLoader
from pesto.plotters import FunctionalPlotter
from pesto.crafters import PerSolverDataCrafter, PerMatrixDataCrafter

def do_plot(config: argparse.Namespace) -> None:
  log = DassLogLoader.run(path=config.logpath)
  FunctionalPlotter.run(log=log, crafter=PerSolverDataCrafter, x='Iteration', y='NormalizedResidual')
  FunctionalPlotter.run(log=log, crafter=PerMatrixDataCrafter, x='Iteration', y='NormalizedResidual')

if __name__ == "__main__":
  action_map: dict[str, typing.Callable[[argparse.Namespace], None]] = {
    'plot': do_plot
  }

  cli = argparse.ArgumentParser()
  cli.add_argument('verb', type=str, choices=action_map.keys(), help='action to perform')
  cli.add_argument('-l', '--logpath', type=str, default='./logs', help='dass log path')
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  config = cli.parse_args(sys.argv[1:])

  action_map[config.verb](config)
