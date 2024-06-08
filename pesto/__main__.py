import typing
import sys
import argparse
import logging
from pesto.loaders import DassLogLoader
from pesto.plotters import FunctionalPlotter
from pesto.crafters import PerSolverDataCrafter, PerMatrixDataCrafter

def do_plot(config: argparse.Namespace) -> None:
  log = DassLogLoader.run(path=config.logpath)
  FunctionalPlotter.run(log=log, crafter=PerSolverDataCrafter, x=config.xmetric, y=config.ymetric, outdir=config.plotpath)
  FunctionalPlotter.run(log=log, crafter=PerMatrixDataCrafter, x=config.xmetric, y=config.ymetric, outdir=config.plotpath)

if __name__ == "__main__":
  action_map: dict[str, typing.Callable[[argparse.Namespace], None]] = {
    'plot': do_plot
  }

  cli = argparse.ArgumentParser()
  cli.add_argument('verb', type=str, choices=action_map.keys(), help='action to perform')
  cli.add_argument('-l', '--logpath', type=str, default='./logs', help='dass log path')
  cli.add_argument('-p', '--plotpath', type=str, default='./plots/logs', help='dass plot path')
  cli.add_argument('-x', '--xmetric', type=str, default='TotalElapsed', choices=['TotalElapsed', 'Iteration'], help='which metric to plot on X axis')
  cli.add_argument('-y', '--ymetric', type=str, default='NormalizedResidual', choices=['NormalizedResidual', 'TotalElapsed', 'Iteration'], help='which metric to plot on Y ayis')
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  config = cli.parse_args(sys.argv[1:])

  if config.verbose:
    logging.getLogger().setLevel(logging.INFO)

  action_map[config.verb](config)
