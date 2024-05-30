#ifndef DASSCC_INDIRECT_SOLVER_HH
#define DASSCC_INDIRECT_SOLVER_HH
#include <dasscc/result.hh>
#include <dasscc/traits.hh>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
/** @file */
namespace dasscc {
  /** trait for Indirect Solver */
  Trait(IndirectSolver,
    Fn(Result<Eigen::SparseVector<double_t>>, run, (Eigen::SparseMatrix<double_t, Eigen::RowMajor>&,
                                                    Eigen::SparseVector<double_t>&,
                                                    double_t, uint32_t)))
}
#endif//DASSCC_INDIRECT_SOLVER_HH
