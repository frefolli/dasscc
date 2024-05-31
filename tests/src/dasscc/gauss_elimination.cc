#include <dasscc/tests.hh>
#include <dasscc/matrix.hh>
#include <dasscc/gauss_elimination_solver.hh>
#include <dasscc/testing.hh>
#include <cassert>

void TestGaussElimination() {
  dasscc::MatrixSpecifier matrix_specifier = {
    .type = dasscc::MatrixSpecifier::Type::SPD,
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
  
  dasscc::GaussEliminationSolver solver;
  dasscc::Result result = solver.run(A, b);
  assert(result.type == result.OK);

  dasscc::Comparison comparison = dasscc::CompareVectors(xe, result.data);
  assert(comparison.same_dimension);
  assert(comparison.norm_of_diff < dasscc::TESTING_ATOL);
  assert(comparison.mean_cwise_diff < dasscc::TESTING_ATOL);
}
