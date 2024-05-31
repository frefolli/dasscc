#include <dasscc/conjugate_gradient_engine.hh>

void dasscc::ConjugateGradientEngine::post_initialize(State& state) {
  state.d_k = state.r_k;
  state.z_k = state.A * state.r_k;
  state.v_k = state.z_k;
}

void dasscc::ConjugateGradientEngine::pre_compute_y(State& state) {
  double_t a = state.d_k.transpose().dot(state.r_k);
  double_t b = state.r_k.transpose().dot(state.v_k);
  state.alpha_k = a / b;
}

void dasscc::ConjugateGradientEngine::compute_y(State& state) {
  state.y = state.alpha_k * state.d_k;
}

void dasscc::ConjugateGradientEngine::post_compute_x(State& state) {
  state.z_n = state.A * state.r_n;
  double_t a = state.d_k.transpose().dot(state.z_n);
  double_t b = state.d_k.transpose().dot(state.v_k);
  state.beta_k = a / b;
  state.d_n = state.r_n - state.beta_k * state.d_k;
  state.v_n = state.A * state.d_n;
}