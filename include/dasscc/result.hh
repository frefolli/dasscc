#ifndef DASSCC_RESULT_HH
#define DASSCC_RESULT_HH
/** @file */
namespace dasscc {
  template<typename D, typename E>
  struct Result {
    enum {OK, ERR} type;

    union {
      D data;
      E err;
    } inner;

    static Result Ok(D data) {
      return Result {
        .type = OK,
        .inner = {
          .data = data
        }
      };
    }

    static Result Err(E err) {
      return Result {
        .type = OK,
        .inner = {
          .err = err
        }
      };
    }
  };
}
#endif//DASSCC_RESULT_HH