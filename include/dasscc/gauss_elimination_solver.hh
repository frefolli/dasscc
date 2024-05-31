#ifndef DASSCC_GAUSS_ELIMINATION_SOLVER_HH
#define DASSCC_GAUSS_ELIMINATION_SOLVER_HH
#include <dasscc/direct_solver.hh>
#include <dasscc/backward_substitution_solver.hh>
/** @file */
namespace dasscc {
  /** trait */
  class GaussEliminationSolver {
    BackwardSubstitutionSolver backward_substitution_solver;
    public:
      Result<Eigen::SparseVector<double_t>> run(
        Eigen::SparseMatrix<double_t, Eigen::RowMajor> &A,
        Eigen::SparseVector<double_t> &b
      );
      ImplTrait(GaussEliminationSolver, DirectSolver);
  };
}
#endif//DASSCC_GAUSS_ELIMINATION_SOLVER_HH