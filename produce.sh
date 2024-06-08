#!/bin/bash
MAXITER=30000

function run_matrix_with_solver() {
  tol=$1
  matrix=$2
  solver=$3
  ./builddir/main.exe -S -v -m src:$matrix -s $solver:$tol:$MAXITER
}

function run_matrix_and_collect_logs() {
  tol=$1
  matrix=$2
  rm -rf /tmp/dasscc/logs
  run_matrix_with_solver $tol $matrix ja
  run_matrix_with_solver $tol $matrix gs
  #run_matrix_with_solver $tol $matrix ri
  run_matrix_with_solver $tol $matrix gr
  run_matrix_with_solver $tol $matrix cg
  mkdir -p ./logs/$tol/$matrix
  cp /tmp/dasscc/logs/*.csv ./logs/$tol/$matrix
  python3 -m pesto plot -l logs/$tol -p plots/logs/$tol -v
}

function run_matrices_with_tol() {
  tol=$1
  run_matrix_and_collect_logs $tol spa1
  run_matrix_and_collect_logs $tol spa2
  run_matrix_and_collect_logs $tol vem1
  run_matrix_and_collect_logs $tol vem2
}

function main() {
  run_matrices_with_tol 10e-4
  run_matrices_with_tol 10e-6
  run_matrices_with_tol 10e-8
  run_matrices_with_tol 10e-10
}
main
