#ifndef DASSCC_RESULT_HH
#define DASSCC_RESULT_HH
/** @file */
namespace dasscc {
  template<typename D>
  struct Result {
    enum {OK, ERR, WARN} type;
    ~Result() {}
    D data;
    
    static Result Ok(D data) {
      return Result {
        .type = OK,
        .data = data
      };
    }

    static Result Err() {
      return Result {
        .type = ERR,
        .data = D()
      };
    }

    static Result Warn(D data) {
      return Result {
        .type = WARN,
        .data = data
      };
    }
  };
}
#endif//DASSCC_RESULT_HH
