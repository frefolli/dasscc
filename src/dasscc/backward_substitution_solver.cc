#include <dasscc/backward_substitution_solver.hh>
#include <dasscc/matrix.hh>

dasscc::Result<Eigen::SparseVector<double_t>> dasscc::BackwardSubstitutionSolver::run(Eigen::SparseMatrix<double_t, Eigen::RowMajor> &A, Eigen::SparseVector<double_t> &b) {
  const int32_t N = A.cols();
  Eigen::SparseVector<double_t> x;
  dasscc::ArrayOfZeros(x, N);

  double_t ai = A.coeff(N - 1, N - 1);

  if ((ai == 0)) {
    return dasscc::Result<Eigen::SparseVector<double_t>>::Err();
  }
  x.coeffRef(N - 1) = b.coeff(N - 1) / ai;

  for (int32_t i = N - 2; i >= 0; --i) {
    ai = A.coeff(i, i);
    if (ai == 0) {
      return dasscc::Result<Eigen::SparseVector<double_t>>::Err();
    }
    double_t d = A.row(i).dot(x);
    x.coeffRef(i) = (b.coeff(i) - d) / ai;
  }
  
  return dasscc::Result<Eigen::SparseVector<double_t>>::Ok(x);
}