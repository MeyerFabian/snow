#ifndef BENCHMARKER_H
#define BENCHMARKER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <assert.h>
#include <chrono>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "defines.hpp"
#include "file_loader.hpp"
#include "statistics.hpp"
class Benchmarker {
 public:
  using BenchmarkerMap =
      std::unordered_map<std::string, std::vector<long long unsigned int>>;

  static void write(BenchmarkerMap&);
};

class BenchmarkerCPU : public Benchmarker {
 public:
  BenchmarkerCPU();
  void write_to_file();

  template <typename F, typename... Args>
  decltype(auto) time(std::string name, F&& func, Args&&... args) {
#ifdef BENCHMARK
    TimerRAIIChrono scoped_timer(name, benchStats);
#endif
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
// Singleton since OpenGL runs on main thread only, ease of use
class BenchmarkerGPU : public Benchmarker {
  using UnprocTimers = std::vector<std::pair<std::string, GLuint>>;
  using UnprocTimersQueue = std::queue<UnprocTimers>;

 public:
  static BenchmarkerGPU& getInstance() {
    static BenchmarkerGPU instance;
    return instance;
  }
  // deactivate copy op, move op
  BenchmarkerGPU(BenchmarkerCPU const&) = delete;
  void operator=(BenchmarkerGPU const&) = delete;

  BenchmarkerGPU(BenchmarkerGPU&&) = delete;
  void operator=(BenchmarkerGPU&&) = delete;

  static void collect_times_last_frame();
  static void write_to_file();

  template <typename F, typename... Args>
  decltype(auto) time(std::string name, F&& func, Args&&... args) {
#ifdef BENCHMARK
    TimerRAIIGL scoped_timer(name);
#endif
    return std::forward<F>(func)(std::forward<Args>(args)...);
  }

 private:
  BenchmarkerGPU();
  static BenchmarkerMap benchStats;
  static UnprocTimersQueue unprocTimers;

  static bool notFirstFrame;
  static void set_not_first_frame();
  class TimerRAIIGL {
   public:
    TimerRAIIGL(std::string name);
    ~TimerRAIIGL() noexcept;
  };
};
#endif  // BENCHMARKER_H

