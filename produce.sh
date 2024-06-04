#!/bin/bash
TOL=10e-4
MAXITER=30000

function run_matrix_with_solver() {
  matrix=$1
  solver=$2
  ./builddir/main.exe -S -v -m src:$matrix -s $solver:$TOL:$MAXITER
}

function run_matrix_and_collect_logs() {
  matrix=$1
  rm -rf /tmp/dasscc/logs
  run_matrix_with_solver "$matrix" ja
  run_matrix_with_solver "$matrix" gs
  #run_matrix_with_solver "$matrix" ri
  run_matrix_with_solver "$matrix" gr
  run_matrix_with_solver "$matrix" cg
  mkdir -p ./logs/$matrix
  cp /tmp/dasscc/logs/*.csv ./logs/$matrix
}

function main() {
  run_matrix_and_collect_logs spa1
  run_matrix_and_collect_logs spa2
  run_matrix_and_collect_logs vem1
  run_matrix_and_collect_logs vem2
}
main
