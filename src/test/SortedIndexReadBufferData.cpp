#include "SortedIndexReadBufferData.hpp"
std::vector<Shader::CommandType> SortedIndexReadBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  vec.insert(
      vec.end(),
      {
          {PreprocessorCmd::DEFINE, define_name + "_SORTING_METHOD INDEX_READ"},

          {PreprocessorCmd::DEFINE, define_name + "_SORTING_KEY" + sorting_key},

      });

  std::string ssbo_define = define_name + "_INDEX";
  auto vec_ssbo = ssbo.data.generateCommands(abstract, ssbo_define);
  vec.insert(vec.end(), vec_ssbo.begin(), vec_ssbo.end());
  return vec;
}

std::unique_ptr<BufferDataInterface>
SortedIndexReadBufferData::cloneBufferDataInterface() {
  return std::make_unique<SortedIndexReadBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexSSBOData(ssbo),
      sorting_key);
}
std::unique_ptr<SortedBufferData> SortedIndexReadBufferData::clone() {
  return std::make_unique<SortedIndexReadBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexSSBOData(ssbo),
      sorting_key);
}

