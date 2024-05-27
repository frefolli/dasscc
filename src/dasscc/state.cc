#include <dasscc/state.hh>

#define AssignKToNAndCleanK(what) \
  what##_k = std::move(what##_n); \
  what##_n.resize(1, 1); \
  what##_n.data().squeeze()

void dasscc::State::update() {
  AssignKToNAndCleanK(x);
  AssignKToNAndCleanK(r);
  AssignKToNAndCleanK(d);
  AssignKToNAndCleanK(z);
  AssignKToNAndCleanK(v);
}