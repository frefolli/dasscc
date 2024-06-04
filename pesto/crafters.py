"""
Crafting Unit
"""
import pesto.generics
import pesto.logs

class PerSolverDataCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.log: pesto.logs.DassLog
    data = {}
    for matrix in self.log.logs:
      matrix_data = self.log.logs[matrix].logs
      for solver in matrix_data:
        solver_data = matrix_data[solver].logs
        if solver not in data:
          data[solver] = {}
        data[solver][matrix] = solver_data
    return data

class PerMatrixDataCrafter(pesto.generics.ServiceObject[dict]):
  def exec(self) -> dict:
    self.log: pesto.logs.DassLog
    data = {}
    for matrix in self.log.logs:
      matrix_data = self.log.logs[matrix].logs
      data[matrix] = {k:matrix_data[k].logs for k in matrix_data}
    return data
