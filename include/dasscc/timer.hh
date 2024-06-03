#ifndef DASSCC_TIMER_HH
#define DASSCC_TIMER_HH
#include <chrono>
#include <string>
#include <cmath>
namespace dasscc {
  struct Timer {
    std::chrono::time_point<std::chrono::utc_clock> internal_clock;

    void reset();
    void round(std::string info);
    double_t round();
  };
}
#endif//DASSCC_TIMER_HH
