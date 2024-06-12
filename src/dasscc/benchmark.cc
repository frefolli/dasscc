#include <dasscc/specifiers.hh>
#include <dasscc/benchmark.hh>
#include <dasscc/logging.hh>
#include <dasscc/timer.hh>
#include <dasscc/matrix.hh>
#include <dasscc/iterative_engine.hh>
#include <dasscc/iterative_solver.hh>
#include <dasscc/jacobi_engine.hh>
#include <dasscc/conjugate_gradient_engine.hh>
#include <dasscc/gauss_seidel_engine.hh>
#include <dasscc/gradient_engine.hh>
#include <dasscc/richardson_engine.hh>
#include <dasscc/testing.hh>
#include <eigen3/Eigen/src/Core/util/Constants.h>
#include <eigen3/Eigen/src/SparseCore/SparseMatrix.h>
#include <json/json.h>
#include <filesystem>
#include <fstream>
#include <cxxabi.h>
#include <json/value.h>
#include <sstream>
#include <string>
#include <cassert>

std::string dasscc::Report::Cell::toKey() const {
  std::ostringstream out;
  out << dasscc::ToString(type);
  if (type == dasscc::MatrixSpecifier::Type::SRC) {
    out << ":" << ID;
  };
  out << ":" << N << ":" << M << ":" << density << ":" << tol;
  return out.str();
}

void dasscc::Report::Cell::includeInfo(const dasscc::MatrixSpecifier& matrix_specifier,
                                       const Eigen::SparseMatrix<double_t, Eigen::RowMajor>& benchmark_matrix,
                                       double_t _tol) {
  type = matrix_specifier.type;
  ID = matrix_specifier.ID;
  N = benchmark_matrix.rows();
  M = benchmark_matrix.cols();
  density = (double_t)benchmark_matrix.nonZeros() / (N * M);
  tol = _tol;
  condition_number = dasscc::ConditionNumber(benchmark_matrix, matrix_specifier);
}

bool dasscc::Report::Cell::operator==(const Cell& other) const {
  return (type == other.type)
      && (ID == other.ID)
      && (density == other.density)
      && (N == other.N)
      && (M == other.M)
      && (tol == other.tol);
}

bool dasscc::LoadReport(dasscc::Report& report, const std::string& filepath) {
  if (!std::filesystem::exists(filepath)) {
    return false;
  }
  report.filepath = filepath;

  std::ifstream in;
  in.open(filepath);
  Json::Value data;
  Json::Reader text_reader;
  if (!(text_reader.parse(in, data))) {
    dasscc::LogWarning("syntax error while reading " + filepath);
    return false;
  }
  in.close();

  if (!data.isObject()) {
    dasscc::LogWarning("expected an object inside " + filepath);
    return false;
  }

  report.data.clear();
  for (auto subtable = data.begin(); subtable != data.end(); ++subtable) {
    std::string key = subtable.key().asString();
    report.data[key] = {};

    if (!subtable->isArray()) {
      dasscc::LogWarning("expected an array as " + subtable->toStyledString() + " children of report inside " + filepath);
      return false;
    }

    for (auto cell = subtable->begin(); cell != subtable->end(); ++cell) {
      if (!cell->isObject()) {
        dasscc::LogWarning("expected objects inside array " + subtable->toStyledString() + " child of report inside " + filepath);
        return false;
      }

      report.data[key].push_back(dasscc::Report::Cell {
        .type = (dasscc::MatrixSpecifier::Type) cell->operator[]("type").asUInt(),
        .ID = cell->operator[]("ID").asString(),
        .N = cell->operator[]("N").asUInt(),
        .M = cell->operator[]("M").asUInt(),
        .density = cell->operator[]("density").asDouble(),
        .tol = cell->operator[]("tol").asDouble(),
        .elapsed = cell->operator[]("elapsed").asDouble(),
        .converged = cell->operator[]("converged").asBool(),
        .error = cell->operator[]("error").asDouble(),
        .condition_number = cell->operator[]("condition_number").asDouble()
      });
    }
  }

  return true;
}

bool dasscc::DumpReport(const dasscc::Report& report) {
  Json::Value dump = Json::objectValue;
  for (std::pair<std::string, std::vector<dasscc::Report::Cell>> subtable : report.data) {
    Json::Value records = Json::arrayValue;
    for (dasscc::Report::Cell cell : subtable.second) {
      Json::Value record = Json::objectValue;
      record["type"] = cell.type;
      record["ID"] = cell.ID;
      record["N"] = cell.N;
      record["M"] = cell.M;
      record["density"] = cell.density;
      record["tol"] = cell.tol;
      record["elapsed"] = cell.elapsed;
      record["converged"] = cell.converged;
      record["error"] = cell.error;
      record["condition_number"] = cell.condition_number;
      records.append(record);
    }
    dump[subtable.first] = records;
  }

  Json::StreamWriterBuilder stream_writer_builder;
  Json::StreamWriter* json_writer = stream_writer_builder.newStreamWriter();
  std::ofstream out;
  out.open(report.filepath);
  json_writer->write(dump, &out);
  out.close();
  delete json_writer;

  return true;
}

bool dasscc::LoadBenchmark(dasscc::Benchmark& benchmark, const std::string& filepath) {
  if (!std::filesystem::exists(filepath)) {
    return false;
  }

  std::ifstream in;
  in.open(filepath);
  Json::Value data;
  Json::Reader text_reader;
  if (!(text_reader.parse(in, data))) {
    dasscc::LogWarning("syntax error while reading " + filepath);
    return false;
  }
  in.close();

  if (!data.isObject()) {
    dasscc::LogWarning("expected an object inside " + filepath);
    return false;
  }

  benchmark.matrix_patterns.clear();
  benchmark.tols.clear();

  Json::Value& matrix_patterns = data["matrix_patterns"];
  for (auto cell = matrix_patterns.begin(); cell != matrix_patterns.end(); ++cell) {
    if (!cell->isString()) {
      dasscc::LogWarning("expected objects inside array \"matrix_patterns\" child of benchmark inside " + filepath);
      return false;
    }
    benchmark.matrix_patterns.push_back(cell->asString());
  }

  Json::Value& tols = data["tols"];
  for (auto cell = tols.begin(); cell != tols.end(); ++cell) {
    if (!cell->isDouble()) {
      dasscc::LogWarning("expected objects inside array \"tols\" child of benchmark inside " + filepath);
      return false;
    }
    benchmark.tols.push_back(cell->asDouble());
  }

  return true;
}

bool dasscc::DumpBenchmark(const dasscc::Benchmark& benchmark, const std::string& filepath) {
  Json::Value dump = Json::objectValue;
  dump["matrix_patterns"] = Json::arrayValue;
  dump["tols"] = Json::arrayValue;

  for (std::string matrix_pattern : benchmark.matrix_patterns) {
    dump["matrix_patterns"].append(matrix_pattern);
  }

  for (double_t tol : benchmark.tols) {
    dump["tols"].append(tol);
  }

  Json::StreamWriterBuilder stream_writer_builder;
  Json::StreamWriter* json_writer = stream_writer_builder.newStreamWriter();
  std::ofstream out;
  out.open(filepath);
  json_writer->write(dump, &out);
  out.close();
  delete json_writer;

  return true;
}

template<typename Engine>
inline void RunAgainstMatrix(dasscc::Report::Cell& result, Eigen::SparseMatrix<double_t, Eigen::RowMajor>& A, double_t tol) {
  ImplTrait(Engine, dasscc::IterativeEngine);
  dasscc::IterativeSolver<Engine> solver;
  uint32_t N = A.cols();
  
  Eigen::SparseVector<double_t> xe;
  dasscc::ArrayOfOnes(xe, N);
  
  Eigen::SparseVector<double_t> b;
  b = A * xe;

  dasscc::Result<Eigen::SparseVector<double_t>> computed;
  dasscc::Timer timer;
  timer.reset();
  computed = solver.run(A, b, tol, 30000);
  result.elapsed = timer.round();
  result.converged = (computed.type == computed.OK);

  dasscc::Comparison comparison = dasscc::CompareVectors(xe, computed.data);
  result.error = comparison.norm_of_diff;
}

template<typename Engine>
inline void RunIfNotAlready(dasscc::Report& report, const dasscc::MatrixSpecifier& matrix_specifier, double_t tol) {
  ImplTrait(Engine, dasscc::IterativeEngine);
  std::string label = abi::__cxa_demangle(typeid(Engine).name(), nullptr, nullptr, nullptr);
  std::vector<dasscc::Report::Cell>& cells = report.data[label];
  Eigen::SparseMatrix<double_t, Eigen::RowMajor> benchmark_matrix;
  assert(dasscc::FromMatrixSpecifier(benchmark_matrix, matrix_specifier));
  dasscc::Report::Cell cell;
  cell.includeInfo(matrix_specifier, benchmark_matrix, tol);

  auto it = std::find(cells.begin(), cells.end(), cell);
  if (it == cells.end()) {
    dasscc::LogInfo("Executing " + cell.toKey() + " for " + label);
    RunAgainstMatrix<Engine>(cell, benchmark_matrix, tol);
    dasscc::LogInfo("Executed  " + cell.toKey() + " for " + label);
    cells.push_back(cell);
  } else {
    dasscc::LogInfo("Skipping  " + cell.toKey() + " for " + label);
  }
}

void dasscc::ExecuteBenchmark(dasscc::Report& report, const dasscc::Benchmark& benchmark) {
  for (std::string matrix_pattern : benchmark.matrix_patterns) {
    dasscc::MatrixSpecifier matrix_specifier = dasscc::ParseMatrixSpecifier(matrix_pattern);
    for (double_t tol : benchmark.tols) {
      // RunIfNotAlready<dasscc::JacobiEngine>(report, matrix_specifier, tol);
      // RunIfNotAlready<dasscc::GaussSeidelEngine>(report, matrix_specifier, tol);
      // RunIfNotAlready<dasscc::RichardsonEngine>(report, matrix_specifier, tol);
      RunIfNotAlready<dasscc::GradientEngine>(report, matrix_specifier, tol);
      RunIfNotAlready<dasscc::ConjugateGradientEngine>(report, matrix_specifier, tol);
    }
  }
}
