#ifndef DASSCC_BENCHMARK_HH
#define DASSCC_BENCHMARK_HH
/** @file benchmark.hh benchmarking utility */
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <cmath>
#include <dasscc/specifiers.hh>
#include <Eigen/Sparse>

namespace dasscc {
  /** Represents a benchmark'in report */
  struct Report {
    /** Represents an entry of a report section */
    struct Cell {
      dasscc::MatrixSpecifier::Type type;
      std::string ID;
      uint32_t N;
      uint32_t M;
      double_t density;
      double_t tol;
      double_t elapsed;
      bool converged;
      double_t error;
      double_t condition_number;

      /** Cell key comprises infos about benchmark, not the results */
      std::string toKey() const;

      /** Collects info about benchmark */
      void includeInfo(const dasscc::MatrixSpecifier& matrix_specifier,
                       const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& benchmark_matrix,
                       double_t tol);

      /** Compares infos about benchmark, not the results */
      bool operator==(const Cell& other) const;
    };

    std::string filepath;
    std::unordered_map<std::string, std::vector<Cell>> data;
  };

  struct Benchmark {
    std::vector<std::string> matrix_patterns;
    std::vector<double_t> tols;
  };

  /**
   * Loads a Report from json file
   * @param report output Report
   * @param filepath input file path
  */
  bool LoadReport(Report& report, const std::string& filepath);
  
  /**
   * Dumps a Report to json file
   * @param report input Report
  */
  bool DumpReport(const Report& report);

  /**
   * Loads a Benchmark from json file
   * @param benchmark output Benchmark
   * @param filepath input file path
  */
  bool LoadBenchmark(Benchmark& benchmark, const std::string& filepath);
  
  /**
   * Dumps a Benchmark to json file
   * @param benchmark input Benchmark
   * @param filepath output file path
  */
  bool DumpBenchmark(const Benchmark& benchmark, const std::string& filepath);

  /**
   * Executes a benchmark and stores info inside the report
   * @param report output Report
   * @param benchmark input Benchmark
  */
  void ExecuteBenchmark(Report& report, const Benchmark& benchmark);
}
#endif//DASSCC_BENCHMARK_HH
