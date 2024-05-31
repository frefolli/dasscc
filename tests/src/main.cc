#include <dasscc/tests.hh>
#include <dasscc/logging.hh>

#define EXECUTE_TEST(Name) \
  dasscc::LogInfo("Testing   " #Name); \
  Test##Name(); \
  dasscc::LogInfo("Done With " #Name);

int main(int argc, char** args) {
  EXECUTE_TEST(BackwardSubstitution)
  EXECUTE_TEST(ForwardSubstitution)
  EXECUTE_TEST(GaussElimination)
  EXECUTE_TEST(JacobiEngine)
  EXECUTE_TEST(GaussSeidelEngine)
  EXECUTE_TEST(RichardsonEngine)
  EXECUTE_TEST(GradientEngine)
  EXECUTE_TEST(ConjugateGradientEngine)
}
