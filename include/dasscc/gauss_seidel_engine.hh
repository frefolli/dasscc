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
      /**
       * Compute Gauss-Seidel P from A
       * @param state input state
      */
      void post_initialize(State& state);
      
      /**
       * Doesn't do anything
       * @param state input state
      */
      void pre_compute_y(State& state);
      
      /**
       * Compute Py = r_k
       * @param state input state
      */
      void compute_y(State& state);
      
      /**
       * Doesn't do anything
       * @param state input state
      */
      void post_compute_x(State& state);
  };

  ImplTrait(GaussSeidelEngine, IterativeEngine);
}
#endif//DASSCC_GAUSS_SEIDEL_ENGINE_HH
