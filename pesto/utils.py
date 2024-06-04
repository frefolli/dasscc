import numpy

def mean(Xs) -> numpy.float64:
  try:
    return numpy.mean(Xs)
  except:
    return numpy.float64(0)

def access_dict(db: dict, keys: list[str]):
  point = db
  for key in keys:
    if point is None:
      return point
    if not isinstance(point, dict):
      return None
    point = point.get(key)
  return point
