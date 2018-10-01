#ifndef MAPREDUCE_H
#define MAPREDUCE_H
#include <execution>
#include <numeric>
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/rendering/GLFWWindow.hpp"
#include "../../snow/shader/technique.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <chrono>
#include <glm/gtx/norm.hpp>
#include <thread>
#include "../../snow/utils/benchmarker.hpp"
#include "../IOBufferData.hpp"
class MapReduceTechnique : public Technique {
 public:
  struct MapReduceData {
    std::string shader_filename;
    LocalSize local_size;
    std::string gl_unary_op;
    std::string gl_binary_op;
    IOBufferData io;
    GLuint numVectors;
  };

  struct DispatchData {
    GLuint bufferSize;
    GLuint dispatchDim_x = 1;
  };

  LocalSize local_size;
  void init(MapReduceData&& data);
  void dispatch_with_barrier(GLuint numVectors) const;
  void dispatch_with_barrier(DispatchData&& data) const;
  void uniforms_update(DispatchData&& uniforms) const;
};
#endif  // MAPREDUCE_H
