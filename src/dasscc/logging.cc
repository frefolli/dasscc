#include <dasscc/logging.hh>
#include <iostream>

void dasscc::RaiseFatalError(std::string message, uint32_t exit_code) {
  std::cerr << dasscc::FATAL_ERROR << message << std::endl;
  exit(exit_code);
}

void dasscc::LogInfo(std::string message) {
  std::cerr << dasscc::INFO << message << std::endl;
}

void dasscc::LogWarning(std::string message) {
  std::cerr << dasscc::WARNING << message << std::endl;
}