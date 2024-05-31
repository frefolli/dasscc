#ifndef DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
#define DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** Engine which implements a Conjugate Gradient version of IterativeEngine */
  class ConjugateGradientEngine {
    public:
      /**
       * Computes d_k, z_k, v_k from A, r_k
       * @param state input state
      */
      void post_initialize(State& state);
      /**
       * Compute alpha_k = (d_k.transpose() * r_k)/(r_k.transpose() * v_k)
       * @param state input state
      */
      void pre_compute_y(State& state);
      /**
       * Compute y = alpha_k * d_k
       * @param state input state
      */
      void compute_y(State& state);
      /**
       * Computes:
       * - z_n from A, r_n
       * - beta_k from d_k, z_n, v_k
       * - d_n from r_n, beta_k, d_k
       * - v_n from A, d_n
       * @param state input state
      */
      void post_compute_x(State& state);
  };

  ImplTrait(ConjugateGradientEngine, IterativeEngine);
}
#endif//DASSCC_CONJUGATE_GRADIENT_ENGINE_HH