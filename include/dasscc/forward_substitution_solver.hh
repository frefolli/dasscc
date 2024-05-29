#ifndef DASSCC_FORWARD_SUBSTITUTION_SOLVER_HH
#define DASSCC_FORWARD_SUBSTITUTION_SOLVER_HH
#include <dasscc/direct_solver.hh>
/** @file */
namespace dasscc {
  /** trait */
  class ForwardSubstitutionSolver {
    public:
      Result<Eigen::SparseVector<double_t>> run(
        Eigen::SparseMatrix<double_t> &A,
        Eigen::SparseVector<double_t> &b
      );
      ImplTrait(ForwardSubstitutionSolver, DirectSolver);
  };
}
#endif//DASSCC_FORWARD_SUBSTITUTION_SOLVER_HH