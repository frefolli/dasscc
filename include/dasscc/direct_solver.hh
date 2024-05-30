#ifndef DASSCC_DIRECT_SOLVER_HH
#define DASSCC_DIRECT_SOLVER_HH
#include <dasscc/result.hh>
#include <dasscc/traits.hh>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
/** @file */
namespace dasscc {
  /** trait */
  Trait(DirectSolver,
    Fn(Result<Eigen::SparseVector<double_t>>, run, (Eigen::SparseMatrix<double_t, Eigen::RowMajor>&,
                                                    Eigen::SparseVector<double_t>&)))
}
#endif//DASSCC_DIRECT_SOLVER_HH