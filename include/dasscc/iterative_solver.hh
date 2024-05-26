#ifndef DASSCC_ITERATIVE_SOLVER_HH
#define DASSCC_ITERATIVE_SOLVER_HH
#include <dasscc/indirect_solver.hh>
#include <dasscc/iterative_engine.hh>
/** @file */
namespace dasscc {
  struct IterativeSolver : public IndirectSolver {
    IterativeEngine& engine;

    Result<Eigen::SparseMatrix<double_t>, Eigen::SparseMatrix<double_t>> run(
      Eigen::SparseMatrix<double_t> &A,
      Eigen::SparseMatrix<double_t> &b,
      double_t tol,
      uint32_t maxIter
    );
  };
}
#endif//DASSCC_ITERATIVE_SOLVER_HH