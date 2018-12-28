#include "StreamCompactIndexBufferData.hpp"
std::vector<Shader::CommandType> StreamCompactIndexBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  vec.insert(vec.end(), {
                            {PreprocessorCmd::DEFINE,
                             define_name + "_STREAM_COMPACT_METHOD INDEX"},

                        });

  std::string ssbo_define = define_name + "_INDEX";
  auto vec_ssbo = ssbo.data.generateCommands(abstract, ssbo_define);
  vec.insert(vec.end(), vec_ssbo.begin(), vec_ssbo.end());
  return vec;
}

std::unique_ptr<BufferDataInterface>
StreamCompactIndexBufferData::cloneBufferDataInterface() {
  return std::make_unique<StreamCompactIndexBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexSSBOData(ssbo));
}
std::unique_ptr<StreamCompactBufferData> StreamCompactIndexBufferData::clone() {
  return std::make_unique<StreamCompactIndexBufferData>(
      buffer_interface->cloneBufferDataInterface(), IndexSSBOData(ssbo));
}

