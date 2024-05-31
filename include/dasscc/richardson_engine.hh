#ifndef DASSCC_RICHARDSON_ENGINE_HH
#define DASSCC_RICHARDSON_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class RichardsonEngine {
    public:
      /**
       * Computes:
       * - Jacobi Q from A
       * - alpha from A
       * @param state input state
      */
      void post_initialize(State& state);
      
      /**
       * Doesn't do anything
       * @param state input state
      */
      void pre_compute_y(State& state);
      
      /**
       * Compute y = alpha * Q * r_k
       * @param state input state
      */
      void compute_y(State& state);
      
      /**
       * Doesn't do anything
       * @param state input state
      */
      void post_compute_x(State& state);
  };

  ImplTrait(RichardsonEngine, IterativeEngine);
}
#endif//DASSCC_RICHARDSON_ENGINE_HH
