#ifndef DASSCC_TESTS
#define DASSCC_TESTS
#include <cmath>
#include <cstdint>

/*
void TestBackwardSubstitution();
void TestForwardSubstitution();
void TestGaussElimination();
void TestJacobiEngine();
void TestGaussSeidelEngine();
void TestRichardsonEngine();
void TestGradientEngine();
void TestConjugateGradientEngine();
*/

namespace dasscc {
  const uint32_t TESTING_MATRIX_SIZE = 50;
  const double_t TESTING_MATRIX_DENSITY = 0.05;
  const double_t TESTING_TOL = 10e-7;
  const uint32_t TESTING_MAXITER = 30000;
  const double_t TESTING_ATOL = 10e-6;
}
#endif//DASSCC_TESTS
