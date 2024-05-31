#include <dasscc/specifiers.hh>

inline void _ProcessBufferForMatrixSpecifier(dasscc::MatrixSpecifier& result, bool& already_read_N, std::string& buffer) {
  switch (result.type) {
    case dasscc::MatrixSpecifier::Type::NONE: {
      if (buffer == "src") {
        result.type = dasscc::MatrixSpecifier::Type::SRC;
      } else if (buffer == "spd") {
        result.type = dasscc::MatrixSpecifier::Type::SPD;
      } else if (buffer == "ut") {
        result.type = dasscc::MatrixSpecifier::Type::UT;
      } else if (buffer == "lt") {
        result.type = dasscc::MatrixSpecifier::Type::LT;
      } else if (buffer == "cdd") {
        result.type = dasscc::MatrixSpecifier::Type::CDD;
      } else if (buffer == "rdd") {
        result.type = dasscc::MatrixSpecifier::Type::RDD;
      }
    }; break;
    case dasscc::MatrixSpecifier::Type::SRC: {
      result.ID = buffer;
    }; break;
    default: {
      if (already_read_N) {
        result.density = std::stod(buffer);
      } else {
        result.N = std::stoi(buffer);
        already_read_N = true;
      }
    }; break;
  }
}

dasscc::MatrixSpecifier dasscc::ParseMatrixSpecifier(std::string pattern) {
  dasscc::MatrixSpecifier result = {
    .type = dasscc::MatrixSpecifier::Type::NONE,
    .ID = "",
    .N = 50,
    .density = 0.05
  };
  bool already_read_N = false;
  std::string buffer;
  for (char c : pattern) {
    if (c == ':') {
      if (buffer.size() > 0) {
        _ProcessBufferForMatrixSpecifier(result, already_read_N, buffer);
        buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
  if (buffer.size() > 0) {
    _ProcessBufferForMatrixSpecifier(result, already_read_N, buffer);
  }
  return result;
}

std::string dasscc::ToString(dasscc::MatrixSpecifier::Type type) {
  constexpr const char* _variants[] = {
    #define X(_) #_
      MATRIX_VARIANTS
    #undef X
  };
  constexpr int _bound = (sizeof(_variants) / sizeof(const char *));

  // Int values can be coerced to component_t
  if (type < _bound)
    return _variants[type];
  return "<matrix-type>";
}

inline void _ProcessBufferForSolverSpecifier(dasscc::SolverSpecifier& result, bool& already_read_tol, std::string& buffer) {
  switch (result.type) {
    case dasscc::SolverSpecifier::Type::NONE: {
      if (buffer == "bs") {
        result.type = dasscc::SolverSpecifier::Type::BS;
      } else if (buffer == "fs") {
        result.type = dasscc::SolverSpecifier::Type::FS;
      } else if (buffer == "ge") {
        result.type = dasscc::SolverSpecifier::Type::GE;
      } else if (buffer == "ja") {
        result.type = dasscc::SolverSpecifier::Type::JA;
      } else if (buffer == "gs") {
        result.type = dasscc::SolverSpecifier::Type::GS;
      } else if (buffer == "ri") {
        result.type = dasscc::SolverSpecifier::Type::RI;
      } else if (buffer == "gr") {
        result.type = dasscc::SolverSpecifier::Type::GR;
      } else if (buffer == "cg") {
        result.type = dasscc::SolverSpecifier::Type::CG;
      }
    }; break;
    default: {
      if (already_read_tol) {
        result.maxIter = std::stoi(buffer);
      } else {
        result.tol = std::stod(buffer);
        already_read_tol = true;
      }
    }; break;
  }
}

dasscc::SolverSpecifier dasscc::ParseSolverSpecifier(std::string pattern) {
  dasscc::SolverSpecifier result = {
    .type = dasscc::SolverSpecifier::Type::NONE,
    .tol = 10e-6,
    .maxIter = 30000
  };

  bool already_read_tol = false;
  std::string buffer;
  for (char c : pattern) {
    if (c == ':') {
      if (buffer.size() > 0) {
        _ProcessBufferForSolverSpecifier(result, already_read_tol, buffer);
        buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
  if (buffer.size() > 0) {
    _ProcessBufferForSolverSpecifier(result, already_read_tol, buffer);
  }
  return result;
}

std::string dasscc::ToString(dasscc::SolverSpecifier::Type type) {
  constexpr const char* _variants[] = {
    #define X(_) #_
      SOLVER_VARIANTS
    #undef X
  };
  constexpr int _bound = (sizeof(_variants) / sizeof(const char *));

  // Int values can be coerced to component_t
  if (type < _bound)
    return _variants[type];
  return "<solver-type>";
}