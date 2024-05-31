#ifndef DASSCC_TESTING_HH
#define DASSCC_TESTING_HH
/** @file testing.hh Testing utilities */
#include <cmath>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/src/Core/util/Constants.h>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>

namespace dasscc {
  struct Comparison {
    bool same_dimension;
    double_t min_cwise_diff;
    double_t max_cwise_diff;
    double_t mean_cwise_diff;
    double_t norm_of_diff;
  };

  Comparison CompareMatrices(Eigen::SparseMatrix<double_t>& A, Eigen::SparseMatrix<double_t>& B);
  Comparison CompareVectors(Eigen::SparseVector<double_t>& A, Eigen::SparseVector<double_t>& B);
}
#endif//DASSCC_TESTING_HH
