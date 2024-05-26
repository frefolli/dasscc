#ifndef DASSCC_GRADIENT_ENGINE_HH
#define DASSCC_GRADIENT_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  struct GradientEngine : IterativeEngine {
    virtual void post_initialize(State& state) = 0;
    virtual void pre_compute_y(State& state) = 0;
    virtual void compute_y(State& state) = 0;
    virtual void post_compute_x(State& state) = 0;
  };
}
#endif//DASSCC_GRADIENT_ENGINE_HH