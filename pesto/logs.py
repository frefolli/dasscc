"""
Dass Logs
"""

class SolverLog:
  """
  Represents the Log of a Solver against a Matrix
  """
  def __init__(self, solver: str, matrix: str, logs: dict[str, list]) -> None:
    self.solver: str = solver
    self.matrix: str = matrix
    self.logs: dict[str, list] = logs

  def __repr__(self) -> str:
    """Returns String representation"""
    return ({k:["..."] for k in self.logs}).__repr__()

class MatrixLog:
  """
  Represents the collection of SolverLogs against a Matrix
  """
  def __init__(self, matrix: str, logs: dict[str, SolverLog]) -> None:
    self.matrix: str = matrix
    self.logs: dict[str, SolverLog] = logs

  def __repr__(self) -> str:
    """Returns String representation"""
    return self.logs.__repr__()

class DassLog:
  """
  Represents the collection of MatrixLogs of Dass
  """
  def __init__(self, logs: dict[str, MatrixLog]) -> None:
    self.logs: dict[str, MatrixLog] = logs

  def __repr__(self) -> str:
    """Returns String representation"""
    return self.logs.__repr__()
