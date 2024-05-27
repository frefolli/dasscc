#ifndef DASSCC_LOGGING_HH
#define DASSCC_LOGGING_HH
#include <string>
#include <cstdint>
/** @file */
namespace dasscc {
#define __BLACK_TEXT "\x1b[30m"
#define __RED_TEXT "\x1b[31m"
#define __GREEN_TEXT "\x1b[32m"
#define __YELLOW_TEXT "\x1b[33m"
#define __BLUE_TEXT "\x1b[34m"
#define __PINK_TEXT "\x1b[35m"
#define __CYAN_TEXT "\x1b[36m"
#define __WHITE_TEXT "\x1b[37m"
#define __GREY_TEXT "\x1b[90m"
#define __LIGHT_RED_TEXT "\x1b[91m"
#define __LIGHT_GREEN_TEXT "\x1b[92m"
#define __LIGHT_YELLOW_TEXT "\x1b[93m"
#define __LIGHT_BLUE_TEXT "\x1b[94m"
#define __LIGHT_PINK_TEXT "\x1b[95m"
#define __LIGHT_CYAN_TEXT "\x1b[96m"
#define __LIGHT_WHITE_TEXT "\x1b[97m"
#define __NORMAL_TEXT "\x1b[0m"

constexpr const char* BLACK_TEXT = __BLACK_TEXT;
constexpr const char* RED_TEXT = __RED_TEXT;
constexpr const char* GREEN_TEXT = __GREEN_TEXT;
constexpr const char* YELLOW_TEXT = __YELLOW_TEXT;
constexpr const char* BLUE_TEXT = __BLUE_TEXT;
constexpr const char* PINK_TEXT = __PINK_TEXT;
constexpr const char* CYAN_TEXT = __CYAN_TEXT;
constexpr const char* WHITE_TEXT = __WHITE_TEXT;
constexpr const char* GREY_TEXT = __GREY_TEXT;
constexpr const char* LIGHT_RED_TEXT = __LIGHT_RED_TEXT;
constexpr const char* LIGHT_GREEN_TEXT = __LIGHT_GREEN_TEXT;
constexpr const char* LIGHT_YELLOW_TEXT = __LIGHT_YELLOW_TEXT;
constexpr const char* LIGHT_BLUE_TEXT = __LIGHT_BLUE_TEXT;
constexpr const char* LIGHT_PINK_TEXT = __LIGHT_PINK_TEXT;
constexpr const char* LIGHT_CYAN_TEXT = __LIGHT_CYAN_TEXT;
constexpr const char* LIGHT_WHITE_TEXT = __LIGHT_WHITE_TEXT;
constexpr const char* NORMAL_TEXT = __NORMAL_TEXT;

constexpr const char* FATAL_ERROR = __RED_TEXT "fatal error: " __NORMAL_TEXT;
constexpr const char* INFO = __YELLOW_TEXT "info: " __NORMAL_TEXT;
constexpr const char* WARNING = __PINK_TEXT "warning: " __NORMAL_TEXT;
constexpr const char* TAB = "  ";

#undef __BLACK_TEXT
#undef __RED_TEXT
#undef __GREEN_TEXT
#undef __YELLOW_TEXT
#undef __BLUE_TEXT
#undef __PINK_TEXT
#undef __CYAN_TEXT
#undef __WHITE_TEXT
#undef __GREY_TEXT
#undef __LIGHT_RED_TEXT
#undef __LIGHT_GREEN_TEXT
#undef __LIGHT_YELLOW_TEXT
#undef __LIGHT_BLUE_TEXT
#undef __LIGHT_PINK_TEXT
#undef __LIGHT_CYAN_TEXT
#undef __LIGHT_WHITE_TEXT
#undef __NORMAL_TEXT
  /**
   * Logs into STDERR a Fatal Error and exit
   * @param message
   * @param exit_code
   * */
  void RaiseFatalError(std::string message, uint32_t exit_code = 1);

  /**
   * Logs into STDERR an Info message
   * @param message
   * */
  void LogInfo(std::string message);

  /**
   * Logs into STDERR a Warning
   * @param message
   * */
  void LogWarning(std::string message);
}
#endif//DASSCC_LOGGING_HH