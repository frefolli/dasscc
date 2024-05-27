#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <dasscc/direct_solver.hh>
#include <dasscc/iterative_solver.hh>
#include <dasscc/jacobi_engine.hh>
#include <cassert>
#include <iostream>

std::string FilenameForSPDMatrix(uint32_t N, uint32_t M) {
  return "resources/matrices/spd/" + std::to_string(N) + "x" + std::to_string(M) + ".mtx";
}

int main(int argc, char** args) {
  uint32_t N = 10;
  double_t density = 0.05;
  if (argc > 1) {
    N = std::stoi(args[1]);
    if (argc > 2) {
      density = std::stod(args[2]);
    }
  }

  Eigen::SparseMatrix<double_t> A;
  dasscc::RandomDiagonalDominant(A, N, density);
  
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;
  
  assert(dasscc::DumpToFile(A, "A.mtx"));
  assert(dasscc::DumpToFile(xe, "xe.mtx"));
  assert(dasscc::DumpToFile(b, "b.mtx"));
  
  dasscc::JacobiEngine engine;
  dasscc::IterativeSolver solver;
  solver.engine = &engine;
  dasscc::Result result = solver.run(A, b, 10e-6, 20000);
  assert(result.type == result.OK);
  
  assert(dasscc::DumpToFile(result.data, "xp.mtx"));
  return 0;
}