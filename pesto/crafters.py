"""
Crafting Unit
"""
import pesto.generics
import pesto.logs
import numpy
import scipy.sparse

class DictOfListsToDictOfArrays(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.payload: dict
    return {k:numpy.array(self.payload[k]) for k in self.payload}

class PerSolverDataCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.input: pesto.logs.DassLog
    data = {}
    for matrix in self.input.logs:
      matrix_data = self.input.logs[matrix].logs
      for solver in matrix_data:
        solver_data = matrix_data[solver].logs
        if solver not in data:
          data[solver] = {}
        data[solver][matrix] = DictOfListsToDictOfArrays.run(payload=solver_data)
    return data

class PerMatrixDataCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.input: pesto.logs.DassLog
    data = {}
    for matrix in self.input.logs:
      matrix_data = self.input.logs[matrix].logs
      data[matrix] = {k:DictOfListsToDictOfArrays.run(payload=matrix_data[k].logs) for k in matrix_data}
    return data

class RowDiagonalDominanceReportCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.matrix: scipy.sparse.csr_matrix = self.matrix.tocsr().__abs__()
    data: dict = {}
    N, M = self.matrix.shape
    data['rapporto_di_dominanza'] = sorted([self.matrix[i, i] / (self.matrix[i].sum() - self.matrix[i, i]) for i in range(N)])
    data['i'] = numpy.array(range(len(data['rapporto_di_dominanza'])))
    return data

class ColumnDiagonalDominanceReportCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.matrix: scipy.sparse.csc_matrix = self.matrix.tocsc().__abs__()
    data: dict = {}
    N, M = self.matrix.shape
    data['rapporto_di_dominanza'] = sorted([self.matrix[i, i] / (self.matrix[:, i].sum() - self.matrix[i, i]) for i in range(M)])
    data['i'] = numpy.array(range(len(data['rapporto_di_dominanza'])))
    return data

class DiagonalDominanceReportCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.matrices: dict
    self.crafter: pesto.generics.ServiceObject[dict]
    reports: dict = {}
    for name in self.matrices:
      reports[name] = self.crafter.run(matrix=self.matrices[name])
    return reports

class BenchmarkReportCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.input: dict
    reports: dict = {}
    for solver in self.input:
      subreport = {}
      for key in self.input[solver][0].keys():
        subreport[key] = [record[key] for record in self.input[solver]]
      reports[solver] = subreport
    return reports
