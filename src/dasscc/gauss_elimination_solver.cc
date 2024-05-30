#include <dasscc/gauss_elimination_solver.hh>
#include <dasscc/matrix.hh>

inline uint32_t max_coeff_in_column(Eigen::SparseMatrix<double_t, Eigen::RowMajor> &A, uint32_t column_index, uint32_t from_row) {
  uint32_t max_index = from_row;
  double_t max_val = A.coeff(from_row, column_index);
  uint32_t M = A.cols();
  for (uint32_t row_index = 0; row_index < M; ++row_index) {
    double_t val = abs(A.coeff(row_index, column_index));
    if (val > max_val) {
      max_index = row_index;
      max_val = val;
    }
  }
  return max_index;
}

inline void swap_rows(Eigen::SparseMatrix<double_t, Eigen::RowMajor> &A, uint32_t row_i, uint32_t row_j) {
  /*
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> perm;
  perm.resize(A.rows(), A.cols());
  perm.reserve(A.rows());
  perm.setIdentity();
  perm.coeffRef(row_i, row_i) = 0;
  perm.coeffRef(row_j, row_j) = 0;
  perm.coeffRef(row_i, row_j) = 1;
  perm.coeffRef(row_i, row_j) = 1;
  */
  auto Ai = A.row(row_i);
  auto Aj = A.row(row_j);
  A.row(row_i) = Aj;
  A.row(row_j) = Ai;
}

dasscc::Result<Eigen::SparseVector<double_t>> dasscc::GaussEliminationSolver::run(Eigen::SparseMatrix<double_t, Eigen::RowMajor> &A, Eigen::SparseVector<double_t> &b) {
  uint32_t M = A.cols();
  for (uint32_t i = 0; i < M - 1; i++) {
    uint32_t j = max_coeff_in_column(A, i, i + 1);
    swap_rows(A, i, j);
    std::swap(b.coeffRef(i), b.coeffRef(j));
    for (uint32_t k = i + 1; k < M; k++) {
      double_t c = A.coeff(k, i) / A.coeff(i, i);
      auto row = A.row(k) - A.row(i) * c;
      A.row(k) = row;
      b.coeffRef(k) -= b.coeff(i) * c;
    }
  }
  return backward_substitution_solver.run(A, b);
}