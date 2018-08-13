#ifndef BENCHMARKER_H
#define BENCHMARKER_H
#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class TimerRAII {
  using BenchmarkerMap =
      std::unordered_map<std::string, std::vector<unsigned int>>;

 public:
  TimerRAII(std::string name, BenchmarkerMap& benchStats)
      : name(name),
        benchStats(benchStats),
        start(std::chrono::high_resolution_clock::now()) {}
  ~TimerRAII() noexcept {
    using namespace std::chrono;
    auto elapsed =
        duration_cast<nanoseconds>(high_resolution_clock::now() - start)
            .count();
    benchStats[name].emplace_back(elapsed);
  }

 private:
  const std::string name;
  BenchmarkerMap& benchStats;
  const std::chrono::high_resolution_clock::time_point start;
};

class Benchmarker {
  using BenchmarkerMap =
      std::unordered_map<std::string, std::vector<unsigned int>>;

 public:
  Benchmarker() noexcept {
    // hacked since first run always takes longer
    benchStats[""].emplace_back(0);
    benchStats.erase(benchStats.begin());
  }

  ~Benchmarker() = default;

  BenchmarkerMap benchStats;

  template <typename F, typename... Args>
  decltype(auto) benchmark_CPU(std::string name, F&& func, Args&&... args) {
    TimerRAII scoped_timer(name, benchStats);
    return std::forward<F>(func)(std::forward<Args>(args)...);
  }
  void printStats() {
    std::cout << "\nBENCHMARKER STATS: (all times in ns)" << std::endl;
    std::cout
        << "Name:\t\t|Max:\t\t|Avg:\t\t|#Calls:\t|Sum:\t\t|Measured Times:"
        << std::endl;
    std::cout << "-------------------------------------------------------------"
                 "---------------------------------"
              << std::endl;
    for (const auto& pair_name_times : benchStats) {
      auto name = pair_name_times.first;
      std::cout << name << "\t\t|";

      auto times = pair_name_times.second;
      auto max = *(std::max_element(times.cbegin(), times.cend()));
      auto sum = std::accumulate(times.cbegin(), times.cend(), 0);
      auto num_calls = times.size();
      auto avg = sum / num_calls;

      std::cout << max << "\t\t|";
      std::cout << avg << "\t\t|";
      std::cout << num_calls << "\t\t|";
      std::cout << sum << "\t\t|";

      for (const auto& time : times) {
        std::cout << time << " ";
      }
      std::cout << std::endl;
    }
  }
};
/* OUTPUT
measuring w/o bench 1936ns

BENCHMARKER STATS: (all times in ns)
Name:		|Max:		|Avg:		|#Calls: |Sum: |Measured Times:
----------------------------------------------------------------------------------------------
id2		|1214		|1214		|1 |1214		|1214 id
|1508		|1348		|2		|2697		|1508 1189
*/

#endif  // BENCHMARKER_H

