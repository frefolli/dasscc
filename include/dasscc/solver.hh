#ifndef DASSCC_SOLVER_HH
#define DASSCC_SOLVER_HH
#include <cmath>
#include <cstdint>
#include <string>
namespace dasscc {
  struct SolverSpecifier {
    enum Type {NONE, BS, FS, GE, JA, GS, RI, GR, CG} type;
    double_t tol;
    uint32_t maxIter;
  };

  SolverSpecifier ParseSolverSpecifier(std::string pattern);
}
#endif//DASSCC_SOLVER_HH
