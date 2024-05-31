#ifndef DASSCC_GAUSS_SEIDEL_ENGINE_HH
#define DASSCC_GAUSS_SEIDEL_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/forward_substitution_solver.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class GaussSeidelEngine {
    dasscc::ForwardSubstitutionSolver forward_substitution_solver;
    public:
      /** TODO: */
      void post_initialize(State& state);
      /** TODO: */
      void pre_compute_y(State& state);
      /** TODO: */
      void compute_y(State& state);
      /** TODO: */
      void post_compute_x(State& state);
  };

  ImplTrait(GaussSeidelEngine, IterativeEngine);
}
#endif//DASSCC_GAUSS_SEIDEL_ENGINE_HH
