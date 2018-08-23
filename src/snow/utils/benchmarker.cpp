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

BenchmarkerGPU::BenchmarkerGPU() {
  // s.t. queue is never empty
  unprocTimers.push(UnprocTimers());

  // caching hack
  benchStats[""].emplace_back(0);
  benchStats.erase(benchStats.begin());
}
bool BenchmarkerGPU::notFirstFrame = false;
void BenchmarkerGPU::set_not_first_frame() {
  BenchmarkerGPU::notFirstFrame = true;
}
/*
 * Queries all relevant GPU data without stalling the gpu
 * (e.g: cpu <-> gpu sync at while(!available)) by waiting one rendered frame.
 */
void BenchmarkerGPU::collect_times_last_frame() {
  if (notFirstFrame) {
    // Therefore unprocTimers always has size 2 after the first frame.
    assert(unprocTimers.size() == 2);

    // Queries of the current frame in the back, queries of the last frame in
    // the front.
    auto toProcessTimers = unprocTimers.front();

    if (!toProcessTimers.empty()) {
      GLint available = 0;
      // Check if last submitted query has become available, then all
      // queries are available.
      while (!available) {
        GLuint lastSubmittedQuery = toProcessTimers.back().second;
        glGetQueryObjectiv(lastSubmittedQuery, GL_QUERY_RESULT_AVAILABLE,
                           &available);
      }

      for (const auto& timer : toProcessTimers) {
        auto name = timer.first;
        auto query = timer.second;
        GLuint64 timeElapsed = 0;

        // See how much time the rendering of object i took in nanoseconds.
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);

        BenchmarkerGPU::benchStats[name].emplace_back(timeElapsed);

        // Free queries so opengl can reuse them.
        glDeleteQueries(1, &query);
      }
    }
    // Current frame becomes last frame for next run
    unprocTimers.pop();
  } else {
    set_not_first_frame();
  }
  unprocTimers.push(UnprocTimers());
}

// This does not look to good, but repeating types is even worse for alias.
decltype(BenchmarkerGPU::unprocTimers) BenchmarkerGPU::unprocTimers =
    UnprocTimersQueue();
decltype(BenchmarkerGPU::benchStats) BenchmarkerGPU::benchStats =
    BenchmarkerMap();

BenchmarkerGPU::TimerRAIIGL::TimerRAIIGL(std::string name) {
  GLuint query;
  glGenQueries(1, &query);
  glBeginQuery(GL_TIME_ELAPSED, query);
  unprocTimers.back().emplace_back(name, query);
}

BenchmarkerGPU::TimerRAIIGL::~TimerRAIIGL() noexcept {
  glEndQuery(GL_TIME_ELAPSED);
}
void BenchmarkerCPU::write_to_file() { Benchmarker::write(benchStats); };
void BenchmarkerGPU::write_to_file() { Benchmarker::write(benchStats); };

void Benchmarker::write(BenchmarkerMap& benchStats) {
  std::stringstream data;
  data << "BENCHMARKER STATS: (all times in ns)\n" << std::endl;
  data << "Name:;Max:;Min:;Avg:;Stdev:;#Calls:\n";
  for (auto& pair_name_times : benchStats) {
    auto name = pair_name_times.first;

    data << name << ";";  // Name
    auto times = pair_name_times.second;

    data << stats::max(times) << ";";  // Max
    data << stats::min(times) << ";";  // Min

    auto avg = stats::avg(times);  // Avg
    data << avg << ";";
    data << stats::stdev(times, avg) << ";";  // Stdev

    data << times.size() << ";";

    data << "\n";
  }
  FileSystem file;
  file.write_to_file(data.str(), "benchstats.csv");
}

