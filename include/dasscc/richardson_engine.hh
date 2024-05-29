#ifndef DASSCC_RICHARDSON_ENGINE_HH
#define DASSCC_RICHARDSON_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class RichardsonEngine {
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

  ImplTrait(RichardsonEngine, IterativeEngine);
}
#endif//DASSCC_RICHARDSON_ENGINE_HH
