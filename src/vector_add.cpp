#include "vector_add.h"
#include <algorithm>

extern "C" void
vector_add(float *out, const float *a, const float *b, int n) {
  std::transform(a, a + n, b, out, std::plus<float>{});
}
