#!/usr/bin/env python3
from collections.abc import Iterable
import json
import argparse
import logging
import sys
import os

def matrix_patterns(config: argparse.Namespace) -> Iterable[str]:
  d = config.mindensity
  while d <= config.maxdensity:
    yield f"{config.kind}:400:{d}"
    if config.ratiodensity > 1:
      d *= config.ratiodensity
    else:
      d += config.stepdensity

def configure(config: argparse.Namespace) -> None:
  conf = {
    'matrix_patterns': list(matrix_patterns(config)), 'tols': [
      1e-8
    ]
  }
  with open("benchmark.json", mode="w") as file:
    json.dump(conf, file)

def preload(config: argparse.Namespace) -> None:
  for matrix in matrix_patterns(config):
    os.system(f"./builddir/main.exe -S -m {matrix} -s \"cg:10e-7:30000\" -e -p")
    logging.info(f"Preloaded {matrix}")

def run(config: argparse.Namespace) -> None:
  os.system("./builddir/main.exe -B -b benchmark.json -r report.json")

def fresh(config: argparse.Namespace) -> None:
  os.system("rm -f report.json")

if __name__ == "__main__":
  cli = argparse.ArgumentParser()
  cli.add_argument('-k', '--kind', type=str, choices=['rdd', 'spd'], default='rdd', help='Minimum Matrix density')
  cli.add_argument('-f', '--mindensity', type=int, default=0.001, help='Minimum Matrix density')
  cli.add_argument('-t','--maxdensity', type=int, default=0.04, help='Maximum Matrix density')
  cli.add_argument('-s','--stepdensity', type=int, default=0.005, help='Step of Matrix densitys')
  cli.add_argument('-r','--ratiodensity', type=int, default=1, help='Ratio of Matrix densities (if enabled, ratiodensity > 1, it overrides stepdensity)')
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  cli.add_argument('-C', '--configure', action='store_true', default=False, help='configure benchmark')
  cli.add_argument('-R', '--run', action='store_true', default=False, help='run benchmark')
  cli.add_argument('-F', '--fresh', action='store_true', default=False, help='remove previous results')
  cli.add_argument('-P', '--preload', action='store_true', default=False, help='preload matrices which are gonna be used')
  config = cli.parse_args(sys.argv[1:])

  print(list(matrix_patterns(config)))

  if config.verbose:
    logging.getLogger().setLevel(logging.INFO)

  if config.configure:
    configure(config)

  if config.fresh:
    fresh(config)

  if config.preload:
    preload(config)

  if config.run:
    run(config)
