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
}

function run_matrices_with_tol() {
  tol=$1
  run_matrix_and_collect_logs $tol spa1
  run_matrix_and_collect_logs $tol spa2
  run_matrix_and_collect_logs $tol vem1
  run_matrix_and_collect_logs $tol vem2
}

function benchmark_of_attached_matrices() {
  #run_matrices_with_tol 1e-4
  python3 -m pesto plot -l logs/1e-4 -p plots/logs/1e-4 -v -s linlog -x Iteration
  #run_matrices_with_tol 1e-6
  python3 -m pesto plot -l logs/1e-6 -p plots/logs/1e-6 -v -s linlog -x Iteration
  #run_matrices_with_tol 1e-8
  python3 -m pesto plot -l logs/1e-8 -p plots/logs/1e-8 -v -s linlog -x Iteration
  #run_matrices_with_tol 1e-10
  python3 -m pesto plot -l logs/1e-10 -p plots/logs/1e-10 -v -r 1e-3 1e-4 1e-6 1e-8 1e-10 1e-11 -s linlog -x Iteration
}

function spy_matrix() {
  matrix=$1
  python3 -m pesto spy -m resources/matrices/$matrix.mtx -o plots/spies/$matrix.png -v
}

function spy_of_attached_matrices() {
  spy_matrix spa1
  spy_matrix spa2
  spy_matrix vem1
  spy_matrix vem2
}

function dominance_reports() {
  python3 -m pesto dom -m resources/matrices -p plots/ -v -d row
  python3 -m pesto dom -m resources/matrices -p plots/ -v -d column
}

function benchmark_rdd() {
  python -m benchmark -CFR -r 2 -v -k spd
  python -m pesto benchmark
  mv plots/logs/benchmark-N-elapsed.png plots/001-benchmark-N-elapsed.png
}

function benchmark_spd() {
  python -m benchmark -CFR -r 2 -v -k rdd
  python -m pesto benchmark
  mv plots/logs/benchmark-N-elapsed.png plots/002-benchmark-N-elapsed.png
}

function conditioning_rdd() {
  python -m conditioning -R -v -s 200 -n 20 -k rdd
  python -m pesto conditioning
  mv plots/logs/conditioning-condition_number-elapsed.png plots/001-conditioning-condition_number-elapsed.png
}

function conditioning_spd() {
  python -m conditioning -R -v -s 400 -n 20 -k spd
  python -m pesto conditioning
  mv plots/logs/conditioning-condition_number-elapsed.png plots/002-conditioning-condition_number-elapsed.png
}

function send_directory_by_filesystem() {
  IN=$1
  OUT=$2
  rm -r $OUT
  cp -r $IN $OUT
}

function deliver_artifacts() {
  send_directory_by_filesystem ./logs /mnt/c/Users/franc/Desktop/dasscc-logs
  send_directory_by_filesystem ./plots /mnt/c/Users/franc/Desktop/dasscc-plots
}

function main() {
  #benchmark_of_attached_matrices
  #dominance_reports
  #spy_of_attached_matrices
  #benchmark_rdd
  #benchmark_spd
  #conditioning_rdd
  conditioning_spd
  deliver_artifacts
}

main
