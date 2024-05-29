#ifndef DASSCC_ITERATIVE_ENGINE_HH
#define DASSCC_ITERATIVE_ENGINE_HH
/** @file */
#include <dasscc/state.hh>
#include <dasscc/traits.hh>
namespace dasscc {
  /** trait for Iterative Engine
   *
   * Example of usage:
   *
   * ```
   * ImplTrait(TheEngine, IterativeEngine);
   * ```
   * */
  Trait(IterativeEngine, 
    Fn(void, post_initialize, (State&))
    Fn(void, pre_compute_y, (State&))
    Fn(void, compute_y, (State&))
    Fn(void, post_compute_x, (State&))
  )
}
#endif//DASSCC_ITERATIVE_ENGINE_HH
