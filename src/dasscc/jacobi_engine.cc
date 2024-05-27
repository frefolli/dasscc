#include <dasscc/jacobi_engine.hh>

void dasscc::JacobiEngine::post_initialize(State& state) {
  const uint32_t N = state.A.rows();
  state.Q.resize(N, N);
  state.Q.reserve(N);
  for (uint32_t i = 0; i < N; ++i) {
    state.Q.coeffRef(i, i) = 1 / state.A.coeff(i, i);
  }
}

void dasscc::JacobiEngine::pre_compute_y(State& /*state*/) {
  // Doesn't do anything
}

void dasscc::JacobiEngine::compute_y(State& state) {
  state.y = state.Q * state.r_k;
}

void dasscc::JacobiEngine::post_compute_x(State& /*state*/) {
  // Doesn't do anything
}