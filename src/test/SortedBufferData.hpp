#ifndef SORTEDBUFFERDATA_HPP_HQMWAVIY
#define SORTEDBUFFERDATA_HPP_HQMWAVIY
#include "BufferData.hpp"

enum class SortingMethod { Full, Index };

struct SortedBufferData {
  SortingMethod sorting_method;
  std::vector<BufferData> buffers;
};

#endif /* end of include guard: SORTEDBUFFERDATA_HPP_HQMWAVIY */

