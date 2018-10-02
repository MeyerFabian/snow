#ifndef COUNTINGTECHNIQUE_H
#define COUNTINGTECHNIQUE_H
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class CountingTechnique : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};

  struct CountingData {
    IOBufferData io;
    GLuint numVectors;  // SOA only
  };

  void init(CountingData&& data);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

