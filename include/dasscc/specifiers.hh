#ifndef DASSCC_SPECIFIERS_HH
#define DASSCC_SPECIFIERS_HH
/** @file specifiers.hh specifier support structures */
#include <string>
#include <cstdint>
#include <cmath>
namespace dasscc {
  #define MATRIX_VARIANTS \
    X(NONE), \
    X(SRC), \
    X(SPD), \
    X(UT), \
    X(LT), \
    X(CDD), \
    X(RDD)
  
  #define X(_) _
  /**
   * Represents a request of matrix
  */
  struct MatrixSpecifier {
    enum Type {
      MATRIX_VARIANTS
    } type;
    std::string ID;
    uint32_t N;
    double_t density;
  };
  #undef X

  /**
   * Returns the string name of the Matrix type
   * @param type input matrix type
  */
  std::string ToString(dasscc::MatrixSpecifier::Type type);

  /**
   * Parses a Matrix specifier from a string pattern
   * @param pattern input pattern
  */
  MatrixSpecifier ParseMatrixSpecifier(std::string pattern);
  
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
  /**
   * Represents a request of solver
  */
  struct SolverSpecifier {
    enum Type { SOLVER_VARIANTS } type;
    double_t tol;
    uint32_t maxIter;
  };
  #undef X
  
  /**
   * Returns the string name of the Solver type
   * @param type input solver type
  */
  std::string ToString(dasscc::SolverSpecifier::Type type);

  /**
   * Parses a Solver specifier from a string pattern
   * @param pattern input pattern
  */
  SolverSpecifier ParseSolverSpecifier(std::string pattern);
}
#endif//DASSCC_SPECIFIERS_HH