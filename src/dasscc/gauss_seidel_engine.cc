#include <dasscc/gauss_seidel_engine.hh>

void dasscc::GaussSeidelEngine::post_initialize(State& state) {
  state.P = state.A.triangularView<Eigen::Lower>();
}

void dasscc::GaussSeidelEngine::pre_compute_y(State& /*state*/) {
  // Doesn't do anything
}

void dasscc::GaussSeidelEngine::compute_y(State& state) {
  dasscc::Result result = forward_substitution_solver.run(state.P, state.r_k);
  assert(result.type == result.OK);
  state.y = result.data;
}

void dasscc::GaussSeidelEngine::post_compute_x(State& /*state*/) {
  // Doesn't do anything
}
