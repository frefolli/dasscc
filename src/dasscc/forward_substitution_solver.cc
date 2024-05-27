#include <dasscc/forward_substitution_solver.hh>
#include <dasscc/matrix.hh>

dasscc::Result<Eigen::SparseVector<double_t>> dasscc::ForwardSubstitutionSolver::run(Eigen::SparseMatrix<double_t> &A, Eigen::SparseVector<double_t> &b) {
  const int32_t N = A.cols();
  Eigen::SparseVector<double_t> x;
  dasscc::ArrayOfZeros(x, N);

  double_t ai = A.coeff(0, 0);

  if ((ai == 0)) {
    return dasscc::Result<Eigen::SparseVector<double_t>>::Err();
  }
  x.coeffRef(0) = b.coeff(0) / ai;

  for (int32_t i = 1; i < N; ++i) {
    ai = A.coeff(i, i);
    if (ai == 0) {
      return dasscc::Result<Eigen::SparseVector<double_t>>::Err();
    }
    double_t d = A.row(i).dot(x);
    x.coeffRef(i) = (b.coeff(i) - d) / ai;
  }
  
  return dasscc::Result<Eigen::SparseVector<double_t>>::Ok(x);
}