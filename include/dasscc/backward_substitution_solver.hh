#ifndef DASSCC_BACKWARD_SUBSTITUTION_SOLVER_HH
#define DASSCC_BACKWARD_SUBSTITUTION_SOLVER_HH
#include <dasscc/direct_solver.hh>
/** @file */
namespace dasscc {
  /** trait */
  struct BackwardSubstitutionSolver : public DirectSolver {
    Result<Eigen::SparseMatrix<double_t>, Eigen::SparseMatrix<double_t>> run(
      Eigen::SparseMatrix<double_t> &A,
      Eigen::SparseMatrix<double_t> &b
    );
  };
}
#endif//DASSCC_BACKWARD_SUBSTITUTION_SOLVER_HH