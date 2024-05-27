#ifndef DASSCC_DIRECT_SOLVER_HH
#define DASSCC_DIRECT_SOLVER_HH
#include <dasscc/result.hh>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
/** @file */
namespace dasscc {
  /** trait */
  class DirectSolver {
    public:
    enum ExitCode {ERR};

    virtual Result<Eigen::SparseVector<double_t>> run(
      Eigen::SparseMatrix<double_t> &A,
      Eigen::SparseVector<double_t> &b
    ) = 0;
  };
}
#endif//DASSCC_DIRECT_SOLVER_HH