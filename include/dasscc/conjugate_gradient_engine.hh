#ifndef DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
#define DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** Engine which implements a Conjugate Gradient version of IterativeEngine */
  class ConjugateGradientEngine {
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

  ImplTrait(ConjugateGradientEngine, IterativeEngine);
}
#endif//DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
