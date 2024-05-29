#ifndef DASSCC_GRADIENT_ENGINE_HH
#define DASSCC_GRADIENT_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class GradientEngine {
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

  ImplTrait(GradientEngine, IterativeEngine);
}
#endif//DASSCC_GRADIENT_ENGINE_HH
