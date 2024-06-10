import typing
import sys
import argparse
import logging
import os
from pesto.loaders import DassLogLoader, MatrixMarketLoader, JsonLoader
from pesto.plotters import FunctionalPlotter, SparseMatrixPlotter
from pesto.crafters import DiagonalDominanceReportCrafter, PerSolverDataCrafter, PerMatrixDataCrafter, ColumnDiagonalDominanceReportCrafter, RowDiagonalDominanceReportCrafter, BenchmarkReportCrafter

def do_plot(config: argparse.Namespace) -> None:
  log = DassLogLoader.run(path=config.logpath)
  FunctionalPlotter.run(input=log, crafter=PerSolverDataCrafter, x=config.xmetric, y=config.ymetric, scale=config.scale, ranges=config.ranges, outdir=config.plotpath)
  FunctionalPlotter.run(input=log, crafter=PerMatrixDataCrafter, x=config.xmetric, y=config.ymetric, scale=config.scale, ranges=config.ranges, outdir=config.plotpath)

def do_spy(config: argparse.Namespace) -> None:
  matrix = MatrixMarketLoader.run(path=config.mmpath)
  SparseMatrixPlotter.run(matrix=matrix, output=config.output)

def do_dom(config: argparse.Namespace) -> None:
  matrices = {}
  for path in os.listdir(config.mmpath):
    if '.mtx' in path:
      name = path.replace('.mtx', '')
      matrices[name] = MatrixMarketLoader.run(path=os.path.join(config.mmpath, path))

  crafters = {
    'row': RowDiagonalDominanceReportCrafter,
    'column': ColumnDiagonalDominanceReportCrafter
  }
  reports = DiagonalDominanceReportCrafter.run(matrices=matrices, crafter=crafters[config.dominance])
  plotdir = os.path.join(config.plotpath, 'dominance')
  FunctionalPlotter.run(input={config.dominance: reports}, crafter=None, x='i', y='rapporto_di_dominanza', scale=config.scale, ranges=None, outdir=plotdir)

def do_benchmark(config: argparse.Namespace) -> None:
  data = JsonLoader.run(path=config.benchmarkpath)
  reports = BenchmarkReportCrafter.run(input=data)
  x='N'
  #y='error'
  y='elapsed'
  FunctionalPlotter.run(input={f"benchmark-{x}-{y}": reports}, crafter=None, x=x, y=y, scale=config.scale, ranges=None, outdir=config.plotpath)

def do_conditioning(config: argparse.Namespace) -> None:
  reports = JsonLoader.run(path=config.conditioningpath)
  x='condition_number'
  #y='error'
  y='elapsed'
  FunctionalPlotter.run(input={f"conditioning-{x}-{y}": reports}, crafter=None, x=x, y=y, scale=config.scale, ranges=None, outdir=config.plotpath)

if __name__ == "__main__":
  action_map: dict[str, typing.Callable[[argparse.Namespace], None]] = {
    'plot': do_plot,
    'spy': do_spy,
    'dom': do_dom,
    'benchmark': do_benchmark,
    'conditioning': do_conditioning
  }

  cli = argparse.ArgumentParser()
  cli.add_argument('verb', type=str, choices=action_map.keys(), help='action to perform')
  cli.add_argument('-c', '--conditioningpath', type=str, default='./report.json', help='dass conditioning path')
  cli.add_argument('-b', '--benchmarkpath', type=str, default='./report.json', help='dass benchmark path')
  cli.add_argument('-l', '--logpath', type=str, default='./logs', help='dass log path')
  cli.add_argument('-p', '--plotpath', type=str, default='./plots/logs', help='dass plot path')
  cli.add_argument('-m', '--mmpath', type=str, help='matrix market file/dir path')
  cli.add_argument('-o', '--output', type=str, help='output of single file plots like actions:spy')
  cli.add_argument('-x', '--xmetric', type=str, default='TotalElapsed', choices=['TotalElapsed', 'Iteration'], help='which metric to plot on X axis')
  cli.add_argument('-y', '--ymetric', type=str, default='NormalizedResidual', choices=['NormalizedResidual', 'TotalElapsed', 'Iteration'], help='which metric to plot on Y ayis')
  cli.add_argument('-s', '--scale', type=str, default='linlin', choices=['linlin', 'linlog', 'loglin', 'loglog'], help='scale of Xs/Ys in the plots')
  cli.add_argument('-r', '--ranges', type=float, nargs='*', help='optionally applies ranges to the plots limiting the output and fixing some axes')
  cli.add_argument('-d', '--dominance', type=str, default='row', choices=['row', 'column'], help='specifies row/column dominance for reports')
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  config = cli.parse_args(sys.argv[1:])

  if config.verbose:
    logging.getLogger().setLevel(logging.INFO)

  action_map[config.verb](config)
