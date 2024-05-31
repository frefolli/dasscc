#include <dasscc/richardson_engine.hh>
#include <dasscc/matrix.hh>

void dasscc::RichardsonEngine::post_initialize(State& state) {
  const uint32_t N = state.A.rows();
  state.Q.resize(N, N);
  state.Q.reserve(N);
  for (uint32_t i = 0; i < N; ++i) {
    state.Q.coeffRef(i, i) = 1 / state.A.coeff(i, i);
  }
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> PinvA = state.Q * state.A;
  double_t smallest, biggest;
  dasscc::Eigenvalues(smallest, biggest, PinvA);
  state.alpha = 2 / (smallest + biggest);
}

void dasscc::RichardsonEngine::pre_compute_y(State& /*state*/) {
  // Doesn't do anything
}

void dasscc::RichardsonEngine::compute_y(State& state) {
  state.y = state.Q * state.r_k * state.alpha;
}

void dasscc::RichardsonEngine::post_compute_x(State& /*state*/) {
  // Doesn't do anything
}
