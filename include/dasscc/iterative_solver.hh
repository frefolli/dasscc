#ifndef DASSCC_ITERATIVE_SOLVER_HH
#define DASSCC_ITERATIVE_SOLVER_HH
#include <dasscc/indirect_solver.hh>
#include <dasscc/iterative_engine.hh>
#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <dasscc/flags.hh>
#include <iostream>
/** @file */
namespace dasscc {
  template<typename Engine>
  class IterativeSolver {
    ImplTrait(Engine, IterativeEngine);
    Engine engine;

    public:
    
      Result<Eigen::SparseVector<double_t>> run(Eigen::SparseMatrix<double_t, Eigen::RowMajor> &A,
                                                Eigen::SparseVector<double_t> &b,
                                                double_t tol,
                                                uint32_t maxIter) {
        dasscc::State state;
        // Load Input
        state.A = A;
        state.b = b;

        // Precompute if possible
        double_t norm_of_b = state.b.norm();

        // Initialize
        const int32_t N = A.cols();
        dasscc::ArrayOfZeros(state.x_k, N);
        state.r_k = state.b - state.A * state.x_k;
        engine.post_initialize(state);

        for (uint32_t k = 1; k <= maxIter; ++k) {
          // Check Tol
          #ifdef DEBUG
          LogInfo("Checking Norm");
          #endif
          if (state.r_k.norm() / norm_of_b < tol)
            return dasscc::Result<Eigen::SparseVector<double_t>>::Ok(state.x_k);

          // Compute Y
          #ifdef DEBUG
          LogInfo("Pre Compute Y");
          #endif
          engine.pre_compute_y(state);

          #ifdef DEBUG
          LogInfo("Compute Y");
          #endif
          engine.compute_y(state);
          
          #ifdef LOGGING
          std::cout << state.y.norm() << std::endl;
          #endif

          // Compute X
          #ifdef DEBUG
          LogInfo("Compute x_n");
          #endif
          state.x_n = state.x_k + state.y;
          
          #ifdef DEBUG
          LogInfo("Compute r_n");
          #endif
          state.r_n = state.b - (state.A * state.x_n);
          
          #ifdef DEBUG
          LogInfo("Post Compute X");
          #endif
          engine.post_compute_x(state);

          // End of Iteration
          state.update();
        } 
        return dasscc::Result<Eigen::SparseVector<double_t>>::Err();
      }
      ImplTrait(IterativeSolver<Engine>, IndirectSolver);
  };
}
#endif//DASSCC_ITERATIVE_SOLVER_HH
