#ifndef DASSCC_JACOBI_ENGINE_HH
#define DASSCC_JACOBI_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/iterative_engine.hh>
namespace dasscc {
  /** trait */
  class JacobiEngine : public IterativeEngine {
    public:
    /**
     * Compute Jacobi Q from A
     * @param state input state
    */
    void post_initialize(State& state);
    
    /**
     * Doesn't do anything
     * @param state input state
    */
    void pre_compute_y(State& state);
    
    /**
     * Compute y = Q * r_k
     * @param state input state
    */
    void compute_y(State& state);

    /**
     * Doesn't do anything
     * @param state input state
    */
    void post_compute_x(State& state);
  };
}
#endif//DASSCC_JACOBI_ENGINE_HH