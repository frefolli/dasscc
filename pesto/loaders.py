"""
Service Loaders
"""
import pesto.generics
import pesto.logs
import yaml
import pandas
import json
import os
import cxxfilt

class YmlLoader(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.path: str
    with open(self.path) as _in:
      return yaml.load(_in, Loader=yaml.Loader)

class JsonLoader(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.path: str
    with open(self.path) as _in:
      return json.load(_in)

class DataFrameLoader(pesto.generics.ServiceObject[pandas.DataFrame]):
  def exec(self) -> pandas.DataFrame:
    self.path: str
    return pandas.read_csv(self.path)

class CsvLoader(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.path: str
    df = DataFrameLoader.run(path=self.path)
    return {k:list(df[k]) for k in df}

class SolverLogLoader(pesto.generics.ServiceObject[pesto.logs.SolverLog]):
  def exec(self) -> pesto.logs.SolverLog:
    self.path: str
    spliced = self.path.split('/')
    solver: str = cxxfilt.demangle(spliced[-1].replace('.csv', ''), external_only=False)
    matrix: str = spliced[-2]
    logs = CsvLoader.run(path=self.path)
    return pesto.logs.SolverLog(solver=solver, matrix=matrix, logs=logs)

class MatrixLogLoader(pesto.generics.ServiceObject[pesto.logs.MatrixLog]):
  def exec(self) -> pesto.logs.MatrixLog:
    self.path: str
    spliced = self.path.split('/')
    matrix: str = spliced[-1]
    logs: dict[str, pesto.logs.SolverLog] = {}
    for logfile in os.listdir(self.path):
      logpath = os.path.join(self.path, logfile)
      log = SolverLogLoader.run(path=logpath)
      logs[log.solver] = log
    return pesto.logs.MatrixLog(matrix=matrix, logs=logs)

class DassLogLoader(pesto.generics.ServiceObject[pesto.logs.DassLog]):
  def exec(self) -> pesto.logs.DassLog:
    self.path: str
    logs: dict[str, pesto.logs.MatrixLog] = {}
    for logfile in os.listdir(self.path):
      logpath = os.path.join(self.path, logfile)
      log = MatrixLogLoader.run(path=logpath)
      logs[log.matrix] = log
    return pesto.logs.DassLog(logs=logs)
