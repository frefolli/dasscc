#include <dasscc/timer.hh>
#include <dasscc/logging.hh>

void dasscc::Timer::reset() {
  internal_clock = std::chrono::utc_clock::now();
}

void dasscc::Timer::round(std::string info) {
  decltype(internal_clock) now = std::chrono::utc_clock::now();
  double_t timediff = std::chrono::duration<double_t>(now - internal_clock).count();
  dasscc::LogInfo("Timer | " + info + " | Elapsed " + std::to_string(timediff) + "s");
  internal_clock = now;
}

double_t dasscc::Timer::round() {
  decltype(internal_clock) now = std::chrono::utc_clock::now();
  double_t timediff = std::chrono::duration<double_t>(now - internal_clock).count();
  internal_clock = now;
  return timediff;
}

void dasscc::Timer::progress(std::string info) {
  decltype(internal_clock) now = std::chrono::utc_clock::now();
  double_t timediff = std::chrono::duration<double_t>(now - internal_clock).count();
  dasscc::LogInfo("Timer | " + info + " | Total Elapsed " + std::to_string(timediff) + "s");
}

double_t dasscc::Timer::progress() {
  decltype(internal_clock) now = std::chrono::utc_clock::now();
  double_t timediff = std::chrono::duration<double_t>(now - internal_clock).count();
  return timediff;
}
