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
#include <string>
#include <cassert>

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
        .density = cell->operator[]("density").asDouble(),
        .tol = cell->operator[]("tol").asDouble(),
        .elapsed = cell->operator[]("elapsed").asDouble(),
        .converged = cell->operator[]("converged").asBool(),
        .error = cell->operator[]("error").asDouble()
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
      record["density"] = cell.density;
      record["tol"] = cell.tol;
      record["elapsed"] = cell.elapsed;
      record["converged"] = cell.converged;
      record["error"] = cell.error;
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
  dasscc::Report::Cell cell;
  cell.type = matrix_specifier.type;
  cell.ID = matrix_specifier.ID;
  cell.density = matrix_specifier.density;
  cell.N = matrix_specifier.N;
  cell.tol = tol;

  auto it = std::find_if(cells.begin(), cells.end(), [&cell](const dasscc::Report::Cell& existing_cell) {
    return (cell.type == existing_cell.type)
        && (cell.ID == existing_cell.ID)
        && (cell.density == existing_cell.density)
        && (cell.N == existing_cell.N)
        && (cell.tol == existing_cell.tol);
  });
  if (it == cells.end()) {
    Eigen::SparseMatrix<double_t, Eigen::RowMajor> benchmark_matrix;
    assert(dasscc::FromMatrixSpecifier(benchmark_matrix, matrix_specifier));
    RunAgainstMatrix<Engine>(cell, benchmark_matrix, tol);
    cells.push_back(cell);
  } else {
  }
}

void dasscc::ExecuteBenchmark(dasscc::Report& report, const dasscc::Benchmark& benchmark) {
  for (std::string matrix_pattern : benchmark.matrix_patterns) {
    dasscc::MatrixSpecifier matrix_specifier = dasscc::ParseMatrixSpecifier(matrix_pattern);
    for (double_t tol : benchmark.tols) {
      RunIfNotAlready<dasscc::JacobiEngine>(report, matrix_specifier, tol);
      RunIfNotAlready<dasscc::GaussSeidelEngine>(report, matrix_specifier, tol);
      RunIfNotAlready<dasscc::RichardsonEngine>(report, matrix_specifier, tol);
      RunIfNotAlready<dasscc::GradientEngine>(report, matrix_specifier, tol);
      RunIfNotAlready<dasscc::ConjugateGradientEngine>(report, matrix_specifier, tol);
    }
  }
}
