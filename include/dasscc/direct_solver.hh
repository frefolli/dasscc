#ifndef DASSCC_DIRECT_SOLVER_HH
#define DASSCC_DIRECT_SOLVER_HH
#include <dasscc/result.hh>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
/** @file */
namespace dasscc {
  /** trait */
  struct DirectSolver {
    virtual Result<Eigen::SparseMatrix<double_t>, Eigen::SparseMatrix<double_t>> run(
      Eigen::SparseMatrix<double_t> &A,
      Eigen::SparseMatrix<double_t> &b
    ) = 0;
  };
}
#endif//DASSCC_DIRECT_SOLVER_HH