#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <dasscc/testing.hh>

dasscc::Comparison dasscc::CompareMatrices(Eigen::SparseMatrix<double_t>& A, Eigen::SparseMatrix<double_t>& B) {
  Comparison result = {
    .same_dimension = false,
    .min_cwise_diff = DBL_MAX,
    .max_cwise_diff = 0.0,
    .mean_cwise_diff = 0.0,
    .norm_of_diff = 0.0,
  };

  uint32_t N = A.rows();
  uint32_t M = A.cols();
  uint32_t length = N * M;

  if (N == B.rows() && M == B.cols()) {
    result.same_dimension = true;
    double_t diff = 0.0;
    for (uint32_t i = 0; i < N; ++i) {
      for (uint32_t j = 0; j < M; ++j) {
        diff = std::abs(A.coeff(i, j) - B.coeff(i, j));
        if (diff > result.max_cwise_diff) {
          result.max_cwise_diff = diff;
        }
        if (diff > result.min_cwise_diff) {
          result.min_cwise_diff = diff;
        }
        result.mean_cwise_diff += diff;
        result.norm_of_diff += diff * diff;
      }
    }
    result.mean_cwise_diff = result.mean_cwise_diff / length;
    result.norm_of_diff = std::sqrt(result.norm_of_diff);
  }

  return result;
}

dasscc::Comparison dasscc::CompareVectors(Eigen::SparseVector<double_t>& A, Eigen::SparseVector<double_t>& B) {
  Comparison result = {
    .same_dimension = false,
    .min_cwise_diff = DBL_MAX,
    .max_cwise_diff = 0.0,
    .mean_cwise_diff = 0.0,
    .norm_of_diff = 0.0,
  };

  uint32_t N = A.rows();
  uint32_t M = A.cols();
  uint32_t length = N * M;

  if (N == B.rows() && M == B.cols()) {
    result.same_dimension = true;
    double_t diff = 0.0;
    for (uint32_t i = 0; i < N; ++i) {
      for (uint32_t j = 0; j < M; ++j) {
        diff = std::abs(A.coeff(i, j) - B.coeff(i, j));
        if (diff > result.max_cwise_diff) {
          result.max_cwise_diff = diff;
        }
        if (diff < result.min_cwise_diff) {
          result.min_cwise_diff = diff;
        }
        result.mean_cwise_diff += diff;
        result.norm_of_diff += diff * diff;
      }
    }
    result.mean_cwise_diff = result.mean_cwise_diff / length;
    result.norm_of_diff = std::sqrt(result.norm_of_diff);
  }

  return result;
}
