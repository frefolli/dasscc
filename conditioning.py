#!/usr/bin/env python3
from collections.abc import Iterable
import json
import argparse
import logging
import sys
import os

def configure(config: argparse.Namespace) -> None:
  conf = {
    'matrix_patterns': [
        f"{config.kind}:{config.size}:{config.density}"
    ], 'tols': [
      1e-8
    ]
  }
  with open("benchmark.json", mode="w") as file:
    json.dump(conf, file)

def round(config: argparse.Namespace) -> None:
  os.system("rm -r ~/.cache/dasscc/*.bin")
  os.system("rm -r report.json")
  os.system("./builddir/main.exe -B -b benchmark.json -r report.json")

def read() -> dict:
  with open("report.json", mode="r") as file:
    return json.load(file)

def write(obj: dict) -> None:
  with open("report.json", mode="w") as file:
    return json.dump(obj, file)

def aggregate(collection: dict, report: dict) -> dict:
  for key in report:
    if key not in collection:
      collection[key] = []
    collection[key].append(report[key][0])
  return collection

def finalize(collection: dict) -> dict:
  result = {}
  for key in collection:
    records = sorted(collection[key], key = lambda record: record['condition_number'])
    result[key] = {
      subkey:[record[subkey] for record in records] for subkey in records[0]
    }
  return result

def run(config: argparse.Namespace) -> None:
  report = {}
  for _ in range(config.trials):
    configure(config)
    round(config)
    report = aggregate(report, read())
  result = finalize(report)
  write(result)

if __name__ == "__main__":
  cli = argparse.ArgumentParser()
  cli.add_argument('-k', '--kind', type=str, choices=['rdd', 'spd'], default='rdd', help='Kind of matrix to use')
  cli.add_argument('-s', '--size', type=int, default=50, help='Size of matrix to use')
  cli.add_argument('-d', '--density', type=float, default=0.1, help='Density of matrix to use')
  cli.add_argument('-n', '--trials', type=int, default=5, help='Number of trials in benchmark')
  cli.add_argument('-v', '--verbose', action='store_true', default=False, help='verbose output')
  cli.add_argument('-R', '--run', action='store_true', default=False, help='run benchmark')
  config = cli.parse_args(sys.argv[1:])

  if config.verbose:
    logging.getLogger().setLevel(logging.INFO)

  if config.run:
    run(config)
