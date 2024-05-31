#ifndef DASSCC_SOLVER_HH
#define DASSCC_SOLVER_HH
#include <cmath>
#include <cstdint>
#include <string>
/** @file solver.hh solver utils */
namespace dasscc {
  #define SOLVER_VARIANTS \
    X(NONE), \
    X(BS), \
    X(FS), \
    X(GE), \
    X(JA), \
    X(GS), \
    X(RI), \
    X(GR), \
    X(CG)
  
  #define X(_) _
  /***/
  struct SolverSpecifier {
    enum Type { SOLVER_VARIANTS } type;
    double_t tol;
    uint32_t maxIter;
  };
  #undef X
  
  /***/
  std::string ToString(dasscc::SolverSpecifier::Type type);
  /***/
  SolverSpecifier ParseSolverSpecifier(std::string pattern);
}
#endif//DASSCC_SOLVER_HH
