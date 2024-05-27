#ifndef DASSCC_RICHARDSON_ENGINE_HH
#define DASSCC_RICHARDSON_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class RichardsonEngine : public IterativeEngine {
    public:
    void post_initialize(State& state);
    void pre_compute_y(State& state);
    void compute_y(State& state);
    void post_compute_x(State& state);
  };
}
#endif//DASSCC_RICHARDSON_ENGINE_HH