#include "SortedIndexWriteBufferData.hpp"
std::vector<Shader::CommandType> SortedIndexWriteBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);
  vec.insert(vec.end(), {
                            {PreprocessorCmd::DEFINE,
                             "UNIFORM_SORTED " + ubo.name + "_sorted"},
                            {PreprocessorCmd::DEFINE,
                             "UNIFORM_UNSORTED " + ubo.name + "_unsorted"},

                            {PreprocessorCmd::INCLUDE, ubo.filename},
                            {PreprocessorCmd::UNDEFINE, "UNIFORM_SORTED "},
                            {PreprocessorCmd::UNDEFINE, "UNIFORM_UNSORTED "},
                            {PreprocessorCmd::DEFINE,
                             define_name + "_SORTING_METHOD INDEX_WRITE"},
                        });

  std::string ssbo_define = define_name + "_UNSORTED_INDEX";
  auto vec_ssbo = ssbo.data.generateCommands(abstract, ssbo_define);
  vec.insert(vec.end(), vec_ssbo.begin(), vec_ssbo.end());

  return vec;
}
std::unique_ptr<BufferDataInterface>
SortedIndexWriteBufferData::cloneBufferDataInterface() {
  return std::make_unique<SortedIndexWriteBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexSSBOData(ssbo),
      IndexUBOData(ubo));
}

