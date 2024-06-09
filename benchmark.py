#!/usr/bin/env python3
import json
import argparse
import logging
import sys
import os

def configure(config: argparse.Namespace) -> None:
  conf = {
    'matrix_patterns': [
      f"{config.kind}:{N}:0.10" for N in range(config.minsize, config.maxsize, config.stepsize)
    ], 'tols': [
      10e-7
    ]
  }
  with open("benchmark.json", mode="w") as file:
    json.dump(conf, file)

def preload(config: argparse.Namespace) -> None:
  for N in range(config.minsize, config.maxsize, config.stepsize):
    os.system(f"./builddir/main.exe -S -m {config.kind}:{N}:0.10 -s \"ja:10e-7:30000\" -e")
    logging.info(f"Preloaded {config.kind}:{N}:0.10")

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
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  cli.add_argument('-C', '--configure', action='store_true', default=False, help='configure benchmark')
  cli.add_argument('-R', '--run', action='store_true', default=False, help='run benchmark')
  cli.add_argument('-F', '--fresh', action='store_true', default=False, help='remove previous results')
  cli.add_argument('-P', '--preload', action='store_true', default=False, help='preload matrices which are gonna be used')
  config = cli.parse_args(sys.argv[1:])

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
