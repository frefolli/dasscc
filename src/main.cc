#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <dasscc/testing.hh>
#include <dasscc/solver.hh>
#include <cassert>
#include <iostream>

int main(int argc, char** args) {
  std::string matrix_pattern = "rdd:50:0.05";
  std::string solver_pattern = "ja:10e-7:30000";
  for (int32_t i = 1; i < argc; ++i) {
    std::string argument = args[i];
    if (argument == "-h" || argument == "--help") {
      std::cerr << "usage: " << args[0] << " [-m <matrix-pattern>] [-s <solver-pattern>]" << std::endl;
      return 0;
    } else if (argument == "-m" || argument == "--matrix") {
      if (i + 1 >= argc) {
        std::cerr << "expected matrix-pattern after " << argument << std::endl;
        return 1;
      }
      argument = args[++i];
      matrix_pattern = argument;
    } else if (argument == "-s" || argument == "--solver") {
      if (i + 1 >= argc) {
        std::cerr << "expected solver-pattern after " << argument << std::endl;
        return 1;
      }
      argument = args[++i];
      solver_pattern = argument;
    }
  }

  dasscc::MatrixSpecifier matrix_specifier = dasscc::ParseMatrixSpecifier(matrix_pattern);
  dasscc::LogInfo("matrix.N: " + std::to_string(matrix_specifier.N));
  dasscc::LogInfo("matrix.density: " + std::to_string(matrix_specifier.density));

  Eigen::SparseMatrix<double_t, Eigen::RowMajor> A;
  assert(dasscc::FromMatrixSpecifier(A, matrix_specifier));
  uint32_t N = A.cols();
  
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;
  
  dasscc::SolverSpecifier solver_specifier = dasscc::ParseSolverSpecifier(solver_pattern);
  dasscc::LogInfo("solver.tol: " + std::to_string(solver_specifier.tol));
  dasscc::LogInfo("solver.maxIter: " + std::to_string(solver_specifier.maxIter));
  /*
  dasscc::IterativeSolver<dasscc::JacobiEngine> solver;
  dasscc::Result result = solver.run(A, b, 10e-7, 30000);
  assert(result.type == result.OK);

  dasscc::Comparison comparison = dasscc::CompareVectors(xe, result.data);
  std::cout << "same_dimension := " << comparison.same_dimension << std::endl;
  std::cout << "mean_cwise_diff := " << comparison.mean_cwise_diff << std::endl;
  std::cout << "min_cwise_diff := " << comparison.min_cwise_diff << std::endl;
  std::cout << "max_cwise_diff := " << comparison.max_cwise_diff << std::endl;
  std::cout << "norm_of_diff := " << comparison.norm_of_diff << std::endl;
  */
  return 0;
}
