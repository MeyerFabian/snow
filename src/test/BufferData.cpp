#include "BufferData.hpp"

std::unique_ptr<BufferDataInterface> BufferData::cloneBufferDataInterface() {
  return std::make_unique<BufferData>(*this);
};

void BufferData::setVariable(std::string in_var) { var = in_var; }

std::string BufferData::getVariable() { return var; }

void BufferData::setName(std::string in_name) { name = in_name; }

std::string BufferData::getName() { return name; }

void BufferData::setIndexBuffer(std::string in_index) {
  bufferIndex = in_index;
}

std::string BufferData::getIndexBuffer() { return bufferIndex; }

GLuint BufferData::getSize() { return bufferSize; }

std::vector<Shader::CommandType> BufferData::generateCommands(
    bool abstract, std::string define_name) {
  std::vector<Shader::CommandType> vec;
  std::string layout = define_name + "_AT ";
  if (BufferLayout::AOS == info.layout) {
    if (abstract) {
      vec.insert(vec.end(), {
                                {PreprocessorCmd::INCLUDE,

                                 "\"" + info.glsl_filename + "\""},
                            });

    } else {
      vec.insert(vec.end(), {
                                {PreprocessorCmd::DEFINE, "AOS_LAYOUT"},

                                {PreprocessorCmd::INCLUDE,
                                 "\"" + info.glsl_filename + "\""},
                                {PreprocessorCmd::UNDEFINE, "AOS_LAYOUT"},

                            });
    }
    layout = layout + "AOS";
  } else {
    vec.insert(vec.end(), {
                              {PreprocessorCmd::INCLUDE,

                               "\"" + info.glsl_filename + "\""},
                          });
    layout = layout + "SOA";
  }
  vec.insert(
      vec.end(),
      {
          {PreprocessorCmd::DEFINE, define_name + " " + name},
          {PreprocessorCmd::DEFINE, define_name + "_VAR " + var},
          {PreprocessorCmd::DEFINE,
           std::string(define_name + "_SIZE ") + std::to_string(bufferSize)},
          {PreprocessorCmd::DEFINE, define_name + "_VAR_SIZE " + var_size_def},
          {PreprocessorCmd::DEFINE, std::string(define_name + "_NUM_BUFFER ") +
                                        std::to_string(bufferNum)},
          {PreprocessorCmd::DEFINE,
           define_name + "_INDEX_BUFFER " + bufferIndex},

          {PreprocessorCmd::DEFINE, layout},

      });
  return vec;
}

