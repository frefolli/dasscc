#ifndef DASSCC_BACKWARD_SUBSTITUTION_SOLVER_HH
#define DASSCC_BACKWARD_SUBSTITUTION_SOLVER_HH
#include <dasscc/direct_solver.hh>
/** @file */
namespace dasscc {
  /** trait */
  class BackwardSubstitutionSolver : public DirectSolver {
    public:
    Result<Eigen::SparseVector<double_t>> run(
      Eigen::SparseMatrix<double_t> &A,
      Eigen::SparseVector<double_t> &b
    );
  };
}
#endif//DASSCC_BACKWARD_SUBSTITUTION_SOLVER_HH