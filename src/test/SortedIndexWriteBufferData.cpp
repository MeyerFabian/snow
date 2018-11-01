#include "SortedIndexWriteBufferData.hpp"
std::vector<Shader::CommandType> SortedIndexWriteBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  // def+undef ssbo_uint
  // def+undef ssbo_binding
  // def+undef ssbo_buffer
  vec.insert(vec.end(), {
                            {PreprocessorCmd::DEFINE,
                             define_name + "_SORTING_METHOD INDEX_WRITE"},
                        });

  std::string ssbo_define = define_name + "_UNSORTED_INDEX ";
  auto vec_ssbo = ssbo.data.generateCommands(abstract, ssbo_define);
  vec.insert(vec.end(), vec_ssbo.begin(), vec_ssbo.end());

  // def+undef ubo_unsorted
  // def+undef ubo_sorted
  // def+undef ubo_binding
  // def+undef ubo_buffer

  return vec;
}
std::unique_ptr<BufferDataInterface>
SortedIndexWriteBufferData::cloneBufferDataInterface() {
  return std::make_unique<SortedIndexWriteBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexSSBOData(ssbo),
      IndexUBOData(ubo));
}

