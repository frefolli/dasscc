#ifndef DASSCC_ITERATIVE_SOLVER_HH
#define DASSCC_ITERATIVE_SOLVER_HH
/** @file */
#include <dasscc/indirect_solver.hh>
#include <dasscc/iterative_engine.hh>
#include <dasscc/matrix.hh>
#include <dasscc/logging.hh>
#include <dasscc/flags.hh>

#ifdef DEBUG
#include <fstream>
#include <string>
#include <filesystem>
#endif

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
        #ifdef DEBUG
        std::string logdir = "/tmp/dasscc";
        std::filesystem::create_directories(logdir);
        std::string logfile = logdir + "/" + typeid(Engine).name();
        std::ofstream out;
        out.open(logfile);
        out << "Iteration,NormalizedResidual" << std::endl;
        #endif
        
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
          double_t normalized_residual = state.r_k.norm() / norm_of_b;
          #ifdef DEBUG
          out << k << "," << normalized_residual << std::endl;
          #endif
          if (normalized_residual < tol) {
            #ifdef DEBUG
            out.close();
            #endif
            return dasscc::Result<Eigen::SparseVector<double_t>>::Ok(state.x_k);
          }

          // Compute Y
          engine.pre_compute_y(state);
          engine.compute_y(state);

          // Compute Xn
          state.x_n = state.x_k + state.y;
          state.r_n = state.b - (state.A * state.x_n);
          engine.post_compute_x(state);

          // End of Iteration
          state.update();
        } 
        #ifdef DEBUG
        out.close();
        #endif
        return dasscc::Result<Eigen::SparseVector<double_t>>::Err();
      }
      ImplTrait(IterativeSolver<Engine>, IndirectSolver);
  };
}
#endif//DASSCC_ITERATIVE_SOLVER_HH
