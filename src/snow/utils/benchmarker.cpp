#include "benchmarker.hpp"

BenchmarkerCPU::BenchmarkerCPU() {
  benchStats[""].emplace_back(0);
  benchStats.erase(benchStats.begin());
}

BenchmarkerCPU::TimerRAIIChrono::TimerRAIIChrono(std::string name,
                                                 BenchmarkerMap& benchStats)
    : name(name),
      benchStats(benchStats),
      start(std::chrono::high_resolution_clock::now()) {}

BenchmarkerCPU::TimerRAIIChrono::~TimerRAIIChrono() noexcept {
  using namespace std::chrono;
  auto elapsed =
      duration_cast<nanoseconds>(high_resolution_clock::now() - start).count();
  benchStats[name].emplace_back(elapsed);
}

bool BenchmarkerGPU::popReady = false;
void BenchmarkerGPU::set_pop_ready() { BenchmarkerGPU::popReady = true; }

BenchmarkerGPU::TimerRAIIGL::TimerRAIIGL(std::string name) {
  GLuint query;
  glGenQueries(1, &query);
  glBeginQuery(GL_TIME_ELAPSED, query);
  unprocTimers.back().emplace_back(name, query);
}

void BenchmarkerGPU::collect_times_last_frame() {
  if (popReady) {
    GLint available = 0;
    auto toProcessTimers = unprocTimers.front();
    // Wait for all results to become available
    while (!available) {
      GLuint lastSubmittedQuery = toProcessTimers.back().second;
      glGetQueryObjectiv(lastSubmittedQuery, GL_QUERY_RESULT_AVAILABLE,
                         &available);
    }

    for (const auto& timer : toProcessTimers) {
      auto name = timer.first;
      auto query = timer.second;
      GLuint64 timeElapsed = 0;
      glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);
      // See how much time the rendering of object i took in nanoseconds.
      BenchmarkerGPU::benchStats[name].emplace_back(timeElapsed);
    }
    unprocTimers.pop();
    unprocTimers.push(UnprocTimers());
  }
  set_pop_ready();
}
BenchmarkerGPU::TimerRAIIGL::~TimerRAIIGL() noexcept {
  glEndQuery(GL_TIME_ELAPSED);
}

void BenchmarkerCPU::print_stats() { Benchmarker::print(benchStats); };
void BenchmarkerGPU::print_stats() { Benchmarker::print(benchStats); };

void Benchmarker::print(const BenchmarkerMap& benchStats) {
  std::cout << "\nBENCHMARKER STATS: (all times in ns)" << std::endl;
  std::cout << "Name:\t\t|Max:\t\t|Avg:\t\t|#Calls:\t|Measured Times:"
            << std::endl;
  std::cout << "-------------------------------------------------------------"
               "---------------------------------"
            << std::endl;
  for (const auto& pair_name_times : benchStats) {
    auto name = pair_name_times.first;
    std::cout << name << "\t\t|";

    auto times = pair_name_times.second;
    auto max = *(std::max_element(times.cbegin(), times.cend()));
    auto sum =
        static_cast<double>(std::accumulate(times.cbegin(), times.cend(), 0));
    auto num_calls = times.size();
    auto avg = sum / num_calls;

    std::cout << max << "\t\t|";
    std::cout << avg << "\t\t|";
    std::cout << num_calls << "\t\t|";

    for (const auto& time : times) {
      std::cout << time << " ";
    }
    std::cout << std::endl;
  }
}

