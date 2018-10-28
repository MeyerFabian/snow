#include "IOBufferData.hpp"

IOBufferData IOBufferData::clone() {
  IOBufferData io;
  for (auto it = in_buffer.begin(); it < in_buffer.end(); it++) {
    io.in_buffer.push_back((*it)->cloneBufferDataInterface());
  }

  for (auto it = out_buffer.begin(); it < out_buffer.end(); it++) {
    io.out_buffer.push_back((*it)->cloneBufferDataInterface());
  }
  return io;
}
std::vector<Shader::CommandType> IOBufferData::generateCommands(bool abstract) {
  std::vector<Shader::CommandType> vec;

  // input buffers
  for (size_t i = 0; i < in_buffer.size(); i++) {
    std::string input = "INPUT";
    if (i != 0) {
      input = input + std::to_string(i + 1);
    }
    auto buffer_commands = in_buffer[i]->generateCommands(abstract, input);
    vec.insert(vec.end(), std::begin(buffer_commands),
               std::end(buffer_commands));
  }

  // output buffers
  for (size_t i = 0; i < out_buffer.size(); i++) {
    std::string output = "OUTPUT";
    if (i != 0) {
      output = output + std::to_string(i + 1);
    }
    auto buffer_commands = out_buffer[i]->generateCommands(abstract, output);
    vec.insert(vec.end(), std::begin(buffer_commands),
               std::end(buffer_commands));
  }
  // return combined commands
  return vec;
}
std::vector<Shader::CommandType> IOBufferData::generateCommands() {
  return generateCommands(false);
}

