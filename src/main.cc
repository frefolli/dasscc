#include <dasscc.hh>
#include <cassert>
#include <iostream>

const std::string DEFAULT_MATRIX_PATTERN = "rdd:50:0.05";
const std::string DEFAULT_SOLVER_PATTERN = "ja:10e-7:30000";

struct CliConfig {
  std::string matrix_pattern = DEFAULT_MATRIX_PATTERN;
  std::string solver_pattern = DEFAULT_SOLVER_PATTERN;
  bool dry_run = false;
  bool verbose = false;
};

inline void PrintHelp(std::string executable) {
  std::cerr << "Usage: " << executable << " [options]" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -m/--matrix <matrix-pattern>          Expects a Matrix Specifer of form `src:<path>` or `<class>[:N[:density]]`" << std::endl;
  std::cerr << "  -s/--solver <solver-pattern>          Expects a Solver Specifer of form `<class>[:tol[:maxIter]]`" << std::endl;
  std::cerr << "  -d/--dry-run                          Exit after parsing specifiers" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Default <matrix-pattern> is `" << DEFAULT_MATRIX_PATTERN << "`" << std::endl;
  std::cerr << "Default <solver-pattern> is `" << DEFAULT_SOLVER_PATTERN << "`" << std::endl;
  exit(0);
}

inline void ParseArguments(int argc, char** args, CliConfig& cli_config) {
  for (int32_t i = 1; i < argc; ++i) {
    std::string argument = args[i];
    if (argument == "-h" || argument == "--help") {
      PrintHelp(args[0]);
    } else if (argument == "-m" || argument == "--matrix") {
      if (i + 1 >= argc) {
        dasscc::RaiseFatalError("expected matrix-pattern after " + argument);
      }
      argument = args[++i];
      cli_config.matrix_pattern = argument;
    } else if (argument == "-s" || argument == "--solver") {
      if (i + 1 >= argc) {
        dasscc::RaiseFatalError("expected solver-pattern after " + argument);
      }
      argument = args[++i];
      cli_config.solver_pattern = argument;
    } else if (argument == "-d" || argument == "--dry-run") {
      cli_config.dry_run = true;
    } else if (argument == "-v" || argument == "--verbose") {
      cli_config.verbose = true;
    }
  }
}

inline void UseSpecifierSolver(dasscc::Result<Eigen::SparseVector<double_t>>& result,
                               dasscc::SolverSpecifier& solver_specifier,
                               Eigen::SparseMatrix<double_t, Eigen::RowMajor>& A,
                               Eigen::SparseVector<double_t>& b) {
  switch (solver_specifier.type) {
    case solver_specifier.JA: {
      dasscc::IterativeSolver<dasscc::JacobiEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case solver_specifier.RI: {
      dasscc::IterativeSolver<dasscc::RichardsonEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case solver_specifier.GS: {
      dasscc::IterativeSolver<dasscc::GaussSeidelEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case solver_specifier.GR: {
      dasscc::IterativeSolver<dasscc::GradientEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case solver_specifier.CG: {
      dasscc::IterativeSolver<dasscc::ConjugateGradientEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case solver_specifier.BS: {
      dasscc::BackwardSubstitutionSolver solver;
      result = solver.run(A, b);
    }; break;
    case solver_specifier.FS: {
      dasscc::ForwardSubstitutionSolver solver;
      result = solver.run(A, b);
    }; break;
    case solver_specifier.GE: {
      dasscc::GaussEliminationSolver solver;
      result = solver.run(A, b);
    }; break;
    case solver_specifier.NONE: {
    }; break;
  }
}

int main(int argc, char** args) {
  CliConfig cli_config;
  ParseArguments(argc, args, cli_config);

  dasscc::MatrixSpecifier matrix_specifier = dasscc::ParseMatrixSpecifier(cli_config.matrix_pattern);
  if (cli_config.verbose) {
    dasscc::LogInfo("matrix.type:    " + dasscc::ToString(matrix_specifier.type));
    dasscc::LogInfo("matrix.N:       " + std::to_string(matrix_specifier.N));
    dasscc::LogInfo("matrix.density: " + std::to_string(matrix_specifier.density));
  }
  
  dasscc::SolverSpecifier solver_specifier = dasscc::ParseSolverSpecifier(cli_config.solver_pattern);
  if (cli_config.verbose) {
    dasscc::LogInfo("solver.type:    " + dasscc::ToString(solver_specifier.type));
    dasscc::LogInfo("solver.tol:     " + std::to_string(solver_specifier.tol));
    dasscc::LogInfo("solver.maxIter: " + std::to_string(solver_specifier.maxIter));
  }

  if (cli_config.dry_run)
    return 0;

  Eigen::SparseMatrix<double_t, Eigen::RowMajor> A;
  assert(dasscc::FromMatrixSpecifier(A, matrix_specifier));
  uint32_t N = A.cols();
  
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;

  dasscc::Result<Eigen::SparseVector<double_t>> result;
  UseSpecifierSolver(result, solver_specifier, A, b);
  assert(result.type == result.OK);

  dasscc::Comparison comparison = dasscc::CompareVectors(xe, result.data);
  std::cout << "same_dimension := " << comparison.same_dimension << std::endl;
  std::cout << "mean_cwise_diff := " << comparison.mean_cwise_diff << std::endl;
  std::cout << "min_cwise_diff := " << comparison.min_cwise_diff << std::endl;
  std::cout << "max_cwise_diff := " << comparison.max_cwise_diff << std::endl;
  std::cout << "norm_of_diff := " << comparison.norm_of_diff << std::endl;

  return 0;
}
