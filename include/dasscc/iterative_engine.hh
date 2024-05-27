#ifndef DASSCC_ITERATIVE_ENGINE_HH
#define DASSCC_ITERATIVE_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
namespace dasscc {
  /** trait */
  class IterativeEngine {
    public:
    virtual void post_initialize(State& state) = 0;
    virtual void pre_compute_y(State& state) = 0;
    virtual void compute_y(State& state) = 0;
    virtual void post_compute_x(State& state) = 0;
  };
}
#endif//DASSCC_ITERATIVE_ENGINE_HH