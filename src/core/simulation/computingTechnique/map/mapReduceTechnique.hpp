#ifndef MAPREDUCE_H
#define MAPREDUCE_H
#include <numeric>
#include "../../../buffer/buffer.hpp"
#include "../../../rendering/GLFWWindow.hpp"
#include "../../../shader/technique.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <chrono>
#include <glm/gtx/norm.hpp>
#include <optional>
#include <thread>
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../utils/benchmarker.hpp"
/*
 * Technique to do a gpu reduction algorithm with a BINARY_OP(left,right). Also
 * supplies map operation (UNARY_OP(x)) for manipulation of input. A technique
 * does one reduction of a buffer by (local_size)*(global_loads_per_thread).
 *
 */
class MapReduceTechnique : public Technique {
 public:
  struct MapReduceData {
    std::string shader_filename;
    LocalSize local_size;
    std::string gl_unary_op_return_type;
    std::string gl_unary_op;
    std::string gl_binary_op_neutral_elem;
    std::string gl_binary_op;
  };

  struct DispatchData {
    std::optional<GLuint> bufferSize;
    GLuint dispatchDim_x = 1;
    std::optional<GLuint> global_loads_per_thread = std::nullopt;
    bool dispatchDim_x_is_uniform = true;
  };

  LocalSize local_size;
  void init(MapReduceData&& data, IOBufferData&&);
  void init(std::vector<Shader::CommandType>&&, MapReduceData&& data,
            IOBufferData&&);
  void dispatch_with_barrier(GLuint numVectors) const;
  void dispatch_with_barrier(DispatchData&& data) const;
  void uniforms_update(DispatchData&& uniforms) const;

 private:
  std::vector<Shader::CommandType> commands;
};
#endif  // MAPREDUCE_H

