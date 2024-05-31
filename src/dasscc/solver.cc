#include <dasscc/solver.hh>

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
        buffer.clear();
      }
    } else {
      buffer += c;
    }
  }
  return result;
}
