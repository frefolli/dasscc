#include <dasscc/solver.hh>

inline void _ProcessBuffer(dasscc::SolverSpecifier& result, bool& already_read_tol, std::string& buffer) {
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
        _ProcessBuffer(result, already_read_tol, buffer);
        buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
  if (buffer.size() > 0) {
    _ProcessBuffer(result, already_read_tol, buffer);
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
