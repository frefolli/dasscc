#ifndef DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
#define DASSCC_CONJUGATE_GRADIENT_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class ConjugateGradientEngine : public IterativeEngine {
    public:
    void post_initialize(State& state);
    void pre_compute_y(State& state);
    void compute_y(State& state);
    void post_compute_x(State& state);
  };
}
#endif//DASSCC_CONJUGATE_GRADIENT_ENGINE_HH