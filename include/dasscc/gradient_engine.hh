#ifndef DASSCC_GRADIENT_ENGINE_HH
#define DASSCC_GRADIENT_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class GradientEngine {
    public:
      /**
       * Doesn't do anything
       * @param state input state
      */
      void post_initialize(State& state);
      
      /**
       * Compute alpha_k = (r_k.transpose() * r_k)/(r_k.transpose() * A * r_k)
       * @param state input state
      */
      void pre_compute_y(State& state);
      
      /**
       * Compute y = alpha_k * r_k
       * @param state input state
      */
      void compute_y(State& state);
      
      /**
       * Doesn't do anything
       * @param state input state
      */
      void post_compute_x(State& state);
  };

  ImplTrait(GradientEngine, IterativeEngine);
}
#endif//DASSCC_GRADIENT_ENGINE_HH
