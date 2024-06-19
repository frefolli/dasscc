#!/usr/bin/env python3
from collections.abc import Iterable
import json
import argparse
import logging
import sys
import os

def matrix_patterns(config: argparse.Namespace) -> Iterable[str]:
  N = config.minsize
  while N <= config.maxsize:
    yield f"{config.kind}:{N}:0.03"
    if config.ratiosize > 1:
      N *= config.ratiosize
    else:
      N += config.stepsize

def configure(config: argparse.Namespace) -> None:
  conf = {
    'matrix_patterns': list(matrix_patterns(config)), 'tols': [
      1e-4
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
  cli.add_argument('-k', '--kind', type=str, choices=['rdd', 'spd'], default='rdd', help='Minimum Matrix Size')
  cli.add_argument('-f', '--minsize', type=int, default=100, help='Minimum Matrix Size')
  cli.add_argument('-t','--maxsize', type=int, default=3000, help='Maximum Matrix Size')
  cli.add_argument('-s','--stepsize', type=int, default=100, help='Step of Matrix Sizes')
  cli.add_argument('-r','--ratiosize', type=int, default=1, help='Ratio of Matrix Sizes (if enabled, ratiosize > 1, it overrides stepsize)')
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
