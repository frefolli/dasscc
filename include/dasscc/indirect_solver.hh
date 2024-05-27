#ifndef DASSCC_INDIRECT_SOLVER_HH
#define DASSCC_INDIRECT_SOLVER_HH
#include <dasscc/result.hh>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
/** @file */
namespace dasscc {
  /** trait */
  class IndirectSolver {
    public:
    virtual Result<Eigen::SparseVector<double_t>> run(
      Eigen::SparseMatrix<double_t> &A,
      Eigen::SparseVector<double_t> &b,
      double_t tol,
      uint32_t maxIter
    ) = 0;
  };
}
#endif//DASSCC_INDIRECT_SOLVER_HH