#include "SortedFullBufferData.hpp"
std::vector<Shader::CommandType> SortedFullBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  vec.insert(vec.end(), {
                            {PreprocessorCmd::DEFINE,
                             define_name + "_SORTING_METHOD FULL"},
                        });

  // def+undef ubo_unsorted
  // def+undef ubo_sorted
  // def+undef ubo_binding
  // def+undef ubo_buffer
  return vec;
}

std::unique_ptr<BufferDataInterface>
SortedFullBufferData::cloneBufferDataInterface() {
  return std::make_unique<SortedFullBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexUBOData(ubo));
}

