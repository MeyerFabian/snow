#include "SortedIndexReadBufferData.hpp"
std::vector<Shader::CommandType> SortedIndexReadBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  vec.insert(vec.end(), {PreprocessorCmd::DEFINE,
                         define_name + "_SORTING_METHOD INDEX_READ"});
  return vec;
}

