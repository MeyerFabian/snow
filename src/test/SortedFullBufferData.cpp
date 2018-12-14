#include "SortedFullBufferData.hpp"
std::vector<Shader::CommandType> SortedFullBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  vec.insert(
      vec.end(),
      {

          {PreprocessorCmd::IFNDEF, std_ext::to_upper(ubo.name + "_sorted")},
          {PreprocessorCmd::DEFINE, std_ext::to_upper(ubo.name + "_sorted")},

          {PreprocessorCmd::DEFINE, "UNIFORM_SORTED " + ubo.name + "_sorted"},
          {PreprocessorCmd::DEFINE,
           "UNIFORM_UNSORTED " + ubo.name + "_unsorted"},
          {PreprocessorCmd::INCLUDE, "\"" + ubo.filename + "\""},
          {PreprocessorCmd::UNDEFINE, "UNIFORM_SORTED"},
          {PreprocessorCmd::UNDEFINE, "UNIFORM_UNSORTED "},
          {PreprocessorCmd::DEFINE, define_name + "_SORTING_METHOD FULL"},

          {PreprocessorCmd::DEFINE, define_name + "_SORTING_KEY" + sorting_key},

          {PreprocessorCmd::ENDIF, ""},
      });
  return vec;
}

std::unique_ptr<BufferDataInterface>
SortedFullBufferData::cloneBufferDataInterface() {
  return std::make_unique<SortedFullBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexUBOData(ubo),
      sorting_key);
}

