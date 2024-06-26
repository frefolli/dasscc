#include <dasscc/tests.hh>
#include <dasscc/matrix.hh>
#include <dasscc/iterative_solver.hh>
#include <dasscc/richardson_engine.hh>
#include <dasscc/testing.hh>
#include <cassert>

int main() {
  dasscc::MatrixSpecifier matrix_specifier = {
    .type = dasscc::MatrixSpecifier::Type::RDD,
    .ID = "",
    .N = dasscc::TESTING_MATRIX_SIZE,
    .density = dasscc::TESTING_MATRIX_DENSITY
  };
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> A;
  assert(dasscc::FromMatrixSpecifier(A, matrix_specifier));
  uint32_t N = A.cols();
  
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;
  
  dasscc::IterativeSolver<dasscc::RichardsonEngine> solver;
  dasscc::Result result = solver.run(A, b, dasscc::TESTING_TOL, dasscc::TESTING_MAXITER);
  assert(result.type == result.OK);

  dasscc::Comparison comparison = dasscc::CompareVectors(xe, result.data);
  assert(comparison.same_dimension);
  assert(comparison.norm_of_diff < dasscc::TESTING_ATOL);
  assert(comparison.mean_cwise_diff < dasscc::TESTING_ATOL);
}
