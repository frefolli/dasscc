#include "dasscc/logging.hh"
#include <dasscc.hh>
#include <cassert>
#include <iostream>

const std::string DEFAULT_MATRIX_PATTERN = "rdd:50:0.05";
const std::string DEFAULT_SOLVER_PATTERN = "ja:10e-7:30000";
const std::string DEFAULT_BENCHMARK_FILEPATH = "benchmark.json";
const std::string DEFAULT_REPORT_FILEPATH = "report.json";

struct CliConfig {
  std::string matrix_pattern = DEFAULT_MATRIX_PATTERN;
  std::string solver_pattern = DEFAULT_SOLVER_PATTERN;
  std::string benchmark_filepath = DEFAULT_BENCHMARK_FILEPATH;
  std::string report_filepath = DEFAULT_REPORT_FILEPATH;
  bool dry_run = false;
  bool empty_run = false;
  bool verbose = false;
  bool solve = false;
  bool benchmark = false;
};

inline bool AskYN() {
  std::cout << "May I do it? [y/N]: ";

  std::string response;
  do {
    std::cin >> response;
    while(response.starts_with(' '))
      response.erase(0, 1);
    if (response == "") {
      return false;
    } else if (response == "Y" || response == "y") {
      return true;
    } else if (response == "N" || response == "n") {
      return false;
    }

    std::cout << "I didn't understand, may I do it? [y/N]: ";
  } while(true);
}

inline void PrintHelp(std::string executable) {
  std::cerr << "Usage: " << executable << " [options]" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -m/--matrix <matrix-pattern>          Expects a Matrix Specifer of form `src:<path>` or `<class>[:N[:density]]`" << std::endl;
  std::cerr << "  -s/--solver <solver-pattern>          Expects a Solver Specifer of form `<class>[:tol[:maxIter]]`" << std::endl;
  std::cerr << "  -r/--report <report-filepath>         Expects a json Report filepath" << std::endl;
  std::cerr << "  -b/--benchmark <benchmark-filepath>   Expects a json Benchmark filepath" << std::endl;
  std::cerr << "  -d/--dry-run                          Exit after parsing specifiers" << std::endl;
  std::cerr << "  -e/--empty-run                        Exit after loading matrix" << std::endl;
  std::cerr << "  -v/--verbose                          Verbose log" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Actions:" << std::endl;
  std::cerr << "  -B/--do-benchmark                     Run a benchmark of Iterative Solvers" << std::endl;
  std::cerr << "  -S/--do-solve                         Run a Solver against a Matrix" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Default settings:" << std::endl;
  std::cerr << "  <matrix-pattern>                      `" << DEFAULT_MATRIX_PATTERN << "`" << std::endl;
  std::cerr << "  <solver-pattern>                      `" << DEFAULT_SOLVER_PATTERN << "`" << std::endl;
  std::cerr << "  <benchmark-filepath>                  `" << DEFAULT_BENCHMARK_FILEPATH << "`" << std::endl;
  std::cerr << "  <report-filepath>                     `" << DEFAULT_REPORT_FILEPATH << "`" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Possible solvers:" << std::endl;
  std::cerr << "  - bs                                  Backward Substitution for Upper Triangular Matrices" << std::endl;
  std::cerr << "  - fs                                  Forward Substitution for Lower Triangular Matrices" << std::endl;
  std::cerr << "  - ge                                  Gauss Elimination" << std::endl;
  std::cerr << "  - ja                                  Jacobi's Iterative Method for Diagonal Dominant Matrices" << std::endl;
  std::cerr << "  - gs                                  Gauss-Seidel's Iterative Method for Diagonal Dominant Matrices" << std::endl;
  std::cerr << "  - ri                                  Richardson's Iterative Method for Diagonal Dominant Matrices" << std::endl;
  std::cerr << "  - gr                                  Gradient's Iterative Method for Symmetric and Positive Defined Matrices" << std::endl;
  std::cerr << "  - cg                                  Conjugate Gradient's Iterative Method for Symmetric and Positive Defined Matrices" << std::endl;
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
    } else if (argument == "-r" || argument == "--report") {
      if (i + 1 >= argc) {
        dasscc::RaiseFatalError("expected report-filepath after " + argument);
      }
      argument = args[++i];
      cli_config.report_filepath = argument;
    } else if (argument == "-b" || argument == "--benchmark") {
      if (i + 1 >= argc) {
        dasscc::RaiseFatalError("expected benchmark-filepath after " + argument);
      }
      argument = args[++i];
      cli_config.benchmark_filepath = argument;
    } else if (argument == "-B" || argument == "--do-benchmark") {
      cli_config.benchmark = true;
    } else if (argument == "-S" || argument == "--do-solve") {
      cli_config.solve = true;
    } else if (argument == "-d" || argument == "--dry-run") {
      cli_config.dry_run = true;
    } else if (argument == "-e" || argument == "--empty-run") {
      cli_config.empty_run = true;
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
    case dasscc::SolverSpecifier::Type::JA: {
      dasscc::IterativeSolver<dasscc::JacobiEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case dasscc::SolverSpecifier::Type::RI: {
      dasscc::IterativeSolver<dasscc::RichardsonEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case dasscc::SolverSpecifier::Type::GS: {
      dasscc::IterativeSolver<dasscc::GaussSeidelEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case dasscc::SolverSpecifier::Type::GR: {
      dasscc::IterativeSolver<dasscc::GradientEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case dasscc::SolverSpecifier::Type::CG: {
      dasscc::IterativeSolver<dasscc::ConjugateGradientEngine> solver;
      result = solver.run(A, b, solver_specifier.tol, solver_specifier.maxIter);
    }; break;
    case dasscc::SolverSpecifier::Type::BS: {
      dasscc::BackwardSubstitutionSolver solver;
      result = solver.run(A, b);
    }; break;
    case dasscc::SolverSpecifier::Type::FS: {
      dasscc::ForwardSubstitutionSolver solver;
      result = solver.run(A, b);
    }; break;
    case dasscc::SolverSpecifier::Type::GE: {
      dasscc::GaussEliminationSolver solver;
      result = solver.run(A, b);
    }; break;
    case dasscc::SolverSpecifier::Type::NONE: {
    }; break;
  }
}

int DoSolve(CliConfig& cli_config) {
  dasscc::MatrixSpecifier matrix_specifier = dasscc::ParseMatrixSpecifier(cli_config.matrix_pattern);
  if (cli_config.verbose) {
    dasscc::LogInfo("matrix.type:    " + dasscc::ToString(matrix_specifier.type));
    dasscc::LogInfo("matrix.ID:       " + matrix_specifier.ID);
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

  if (cli_config.verbose)
    dasscc::LogInfo("Loading Matrix ...");
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> A;
  assert(dasscc::FromMatrixSpecifier(A, matrix_specifier));
  uint32_t N = A.cols();
  if (cli_config.verbose)
    dasscc::LogInfo("Loaded Matrix");

  if (cli_config.empty_run)
    return 0;
  
  if (cli_config.verbose)
    dasscc::LogInfo("Creating Example ...");
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  if (cli_config.verbose)
    dasscc::LogInfo("Created Example");
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;

  if (cli_config.verbose)
    dasscc::LogInfo("Solving Matrix ...");
  dasscc::Result<Eigen::SparseVector<double_t>> result;
  UseSpecifierSolver(result, solver_specifier, A, b);
  assert(result.type == result.OK);
  if (cli_config.verbose)
    dasscc::LogInfo("Solved Matrix");

  dasscc::Comparison comparison = dasscc::CompareVectors(xe, result.data);
  std::cout << "same_dimension := " << comparison.same_dimension << std::endl;
  std::cout << "mean_cwise_diff := " << comparison.mean_cwise_diff << std::endl;
  std::cout << "min_cwise_diff := " << comparison.min_cwise_diff << std::endl;
  std::cout << "max_cwise_diff := " << comparison.max_cwise_diff << std::endl;
  std::cout << "norm_of_diff := " << comparison.norm_of_diff << std::endl;

  return 0;
}

int DoBenchmark(CliConfig& cli_config) {
  dasscc::Report report;
  // If doesn't exist, it will create it
  if (!dasscc::LoadReport(report, cli_config.report_filepath)) {
    dasscc::LogInfo("file " + cli_config.report_filepath + " doesn't exist, creating it");
    report.data.clear();
    report.filepath = cli_config.report_filepath;
  }

  dasscc::Benchmark benchmark;
  if (!dasscc::LoadBenchmark(benchmark, cli_config.benchmark_filepath)) {
    dasscc::LogInfo("file " + cli_config.benchmark_filepath + " doesn't exist, defaulting and creating it, if you wish");
    if (AskYN()) {
      benchmark = {
        .matrix_patterns = {"rdd:50:0.10"},
        .tols = {10e-7}
      };
      dasscc::DumpBenchmark(benchmark, cli_config.benchmark_filepath);
      dasscc::LogInfo("wrote " + cli_config.benchmark_filepath);
    } else {
      dasscc::LogInfo("aborting");
    }
  }

  if (cli_config.dry_run)
    return 0;

  dasscc::ExecuteBenchmark(report, benchmark);
  dasscc::DumpReport(report);
  return 0;
}

int main(int argc, char** args) {
  CliConfig cli_config;
  ParseArguments(argc, args, cli_config);

  if (cli_config.benchmark) {
    DoBenchmark(cli_config);
  } else if (cli_config.solve) {
    DoSolve(cli_config);
  }

  return 0;
}
