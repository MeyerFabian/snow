#ifndef STATISTICS_H
#define STATISTICS_H
#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
namespace stats {

template <typename Container>
decltype(auto) stdev(Container&& c, double avg) {
  double accum = 0.0;
  std::for_each(std::cbegin(c), std::cend(c),
                [&](const double d) { accum += (d - avg) * (d - avg); });

  return sqrt(accum / (c.size() - 1));
}

template <typename Container>
decltype(auto) avg(Container&& c) {
  return ((double)std::accumulate(std::cbegin(c), std::cend(c), 0.0)) /
         std::size(c);
}

template <typename Container>
decltype(auto) max(Container&& c) {
  return *(std::max_element(std::cbegin(c), std::cend(c)));
}

template <typename Container>
decltype(auto) min(Container&& c) {
  return *(std::min_element(std::cbegin(c), std::cend(c)));
}

}  // namespace stats
#endif

