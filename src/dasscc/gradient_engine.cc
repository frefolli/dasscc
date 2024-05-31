#include <dasscc/gradient_engine.hh>

void dasscc::GradientEngine::post_initialize(State& /*state*/) {
  // Doesn't do anything
}

void dasscc::GradientEngine::pre_compute_y(State& state) {
  double_t a = state.r_k.transpose().dot(state.r_k);
  double_t b = state.r_k.transpose().dot(state.A * state.r_k);
  state.alpha_k = a / b;
}

void dasscc::GradientEngine::compute_y(State& state) {
  state.y = state.alpha_k * state.r_k;
}

void dasscc::GradientEngine::post_compute_x(State& /*state*/) {
  // Doesn't do anything
}
