#ifndef BENCHMARKER_H
#define BENCHMARKER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Benchmarker {
 public:
  using BenchmarkerMap =
      std::unordered_map<std::string, std::vector<long long unsigned int>>;

  static void print(const BenchmarkerMap&);
};

class BenchmarkerCPU : public Benchmarker {
 public:
  BenchmarkerCPU();
  void print_stats();

  template <typename F, typename... Args>
  decltype(auto) time(std::string name, F&& func, Args&&... args) {
    TimerRAIIChrono scoped_timer(name, benchStats);
    return std::forward<F>(func)(std::forward<Args>(args)...);
  }

 private:
  BenchmarkerMap benchStats;
  class TimerRAIIChrono {
   public:
    TimerRAIIChrono(std::string name, BenchmarkerMap& benchStats);
    ~TimerRAIIChrono() noexcept;

   private:
    const std::string name;
    BenchmarkerMap& benchStats;
    const std::chrono::high_resolution_clock::time_point start;
  };
};

class BenchmarkerGPU : public Benchmarker {
  using UnprocTimers = std::vector<std::pair<std::string, GLuint>>;
  using UnprocTimersQueue = std::queue<UnprocTimers>;

 public:
  BenchmarkerGPU();
  static void print_stats();
  static void collect_times_last_frame();

  template <typename F, typename... Args>
  decltype(auto) time(std::string name, F&& func, Args&&... args) {
    TimerRAIIGL scoped_timer(name);
    return std::forward<F>(func)(std::forward<Args>(args)...);
  }

 private:
  static BenchmarkerMap benchStats;
  static UnprocTimersQueue unprocTimers;
  static bool popReady;
  static void set_pop_ready();
  class TimerRAIIGL {
   public:
    TimerRAIIGL(std::string name);
    ~TimerRAIIGL() noexcept;
  };
};
#endif  // BENCHMARKER_H

