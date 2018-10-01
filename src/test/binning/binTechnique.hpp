#ifndef BINNINGTECHNIQUE_H
#define BINNINGTECHNIQUE_H
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class BinningTechnique : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};

  struct BinningData {
    IOBufferData io;
    GLuint numVectors;  // SOA only
  };

  void init(BinningData&& data);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

