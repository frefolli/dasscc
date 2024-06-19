#!/bin/bash
import os
import sys
import struct

def parse_details(blob: bytes) -> dict:
  data = struct.unpack("@d", blob)
  return {
    'conditioning': data[0]
  }

def read_details(path: str) -> dict:
  with open(path, mode="rb") as file:
    blob = file.read()
    return parse_details(blob)

if __name__ == "__main__":
  for path in sys.argv[1:]:
    print(path, "=>", read_details(os.path.normpath(path)))
