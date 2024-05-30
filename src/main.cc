#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <dasscc/gauss_elimination_solver.hh>
#include <cassert>
#include <iostream>

int main(int argc, char** args) {
  std::string matrix_specifier = "rdd:50:0.05";
  for (int32_t i = 1; i < argc; ++i) {
    std::string argument = args[i];
    if (argument == "-h" || argument == "--help") {
      std::cerr << "usage: " << args[0] << " [-m <matrix-specifier>]" << std::endl;
      return 0;
    } else if (argument == "-m" || argument == "--matrix") {
      if (i + 1 >= argc) {
        std::cerr << "expected matrix-specifier after -m" << std::endl;
        return 1;
      }
      argument = args[++i];
      matrix_specifier = argument;
    }
  }

  Eigen::SparseMatrix<double_t, Eigen::RowMajor> A;
  assert(dasscc::FromMatrixSpecifier(A, matrix_specifier));
  uint32_t N = A.cols();
  
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;
  
  assert(dasscc::DumpToFile(A, "A.mtx"));
  assert(dasscc::DumpToFile(xe, "xe.mtx"));
  assert(dasscc::DumpToFile(b, "b.mtx"));
  
  dasscc::GaussEliminationSolver solver;
  dasscc::Result result = solver.run(A, b);
  assert(result.type == result.OK);
  
  assert(dasscc::DumpToFile(result.data, "xp.mtx"));
  return 0;
}