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
class MapReduce : public Technique {
 public:
  struct MapReduceData {
    std::string shader_filename;
    LocalSize local_size;
    std::string gl_unary_op;
    std::string gl_binary_op;
    std::string input;
    std::string output;
    std::string buffer_filename;
  };
  LocalSize local_size;
  void init(MapReduceData&& data) {
    local_size = data.local_size;

    auto shader =
        std::make_shared<Shader>(ShaderType::COMPUTE, data.shader_filename);

    shader->set_local_size(data.local_size);

    std::vector<Shader::CommandType> vec = {
        {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
        {PreprocessorCmd::DEFINE, "BINARY_OP(left,right) " + data.gl_binary_op},
        {PreprocessorCmd::DEFINE, "UNARY_OP_RETURN_TYPE float"},
        {PreprocessorCmd::DEFINE, "INPUT(value) " + data.input},
        {PreprocessorCmd::DEFINE, "OUTPUT(value) " + data.output},
        {PreprocessorCmd::INCLUDE, "\"" + data.buffer_filename + "\""}};
    shader->add_cmds(vec.begin(), vec.end());

    Technique::add_shader(std::move(shader));
    Technique::upload();
    Technique::use();
  }
  void dispatch_with_barrier(GLuint numVectors) const {
    glDispatchCompute(numVectors / local_size.x, 1 / local_size.y,
                      1 / local_size.z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
  struct DispatchData {
    GLuint maxGlobalInvocationIndex;
    GLuint gridDim_x = 1;
  };
  void dispatch_with_barrier(GLuint dispatch_size, DispatchData&& data) const {
    Technique::use();
    GLuint size = data.maxGlobalInvocationIndex;
    uniforms_update(std::move(data));
    glDispatchCompute(dispatch_size / local_size.x, 1 / local_size.y,
                      1 / local_size.z);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
  void uniforms_update(DispatchData&& uniforms) const {
    Technique::uniform_update("dispatchDim_x", uniforms.gridDim_x);
    Technique::uniform_update("maxGlobalInvocationIndex",
                              uniforms.maxGlobalInvocationIndex);
  }
};
#endif  // MAPREDUCE_H

