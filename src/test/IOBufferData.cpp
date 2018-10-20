#include "IOBufferData.hpp"
std::vector<Shader::CommandType> IOBufferData::generateCommands(bool abstract) {
  std::vector<Shader::CommandType> vec;

  // input buffers
  for (size_t i = 0; i < in_buffer.size(); i++) {
    std::string input = "INPUT";
    if (i != 0) {
      input = input + std::to_string(i + 1);
    }
    std::string layout = input + "_AT ";
    if (BufferLayout::AOS == in_buffer[i].info.layout) {
      if (abstract) {
        vec.insert(vec.end(),
                   {
                       {PreprocessorCmd::INCLUDE,

                        "\"" + in_buffer[i].info.glsl_filename + "\""},
                   });

      } else {
        vec.insert(vec.end(),
                   {
                       {PreprocessorCmd::DEFINE, "AOS_LAYOUT"},

                       {PreprocessorCmd::INCLUDE,
                        "\"" + in_buffer[i].info.glsl_filename + "\""},
                       {PreprocessorCmd::UNDEFINE, "AOS_LAYOUT"},

                   });
      }
      layout = layout + "AOS";
    } else {
      vec.insert(vec.end(), {
                                {PreprocessorCmd::INCLUDE,

                                 "\"" + in_buffer[i].info.glsl_filename + "\""},
                            });
      layout = layout + "SOA";
    }
    vec.insert(
        vec.end(),
        {
            {PreprocessorCmd::DEFINE, input + " " + in_buffer[i].name},
            {PreprocessorCmd::DEFINE, input + "_VAR " + in_buffer[i].var},
            {PreprocessorCmd::DEFINE,
             std::string(input + "_SIZE ") +
                 std::to_string(in_buffer[i].bufferSize)},
            {PreprocessorCmd::DEFINE, std::string(input + "VAR_SIZE ") +
                                          std::to_string(in_buffer[i].varSize)},
            {PreprocessorCmd::DEFINE,
             std::string(input + "_NUM_BUFFER ") +
                 std::to_string(in_buffer[i].bufferNum)},
            {PreprocessorCmd::DEFINE,
             std::string(input + "_INDEX_BUFFER ") +
                 std::to_string(in_buffer[i].bufferIndex)},

            {PreprocessorCmd::DEFINE, layout},

        });
  }

  // output buffers
  for (size_t i = 0; i < out_buffer.size(); i++) {
    std::string output = "OUTPUT";
    if (i != 0) {
      output = output + std::to_string(i + 1);
    }
    std::string layout = output + "_AT ";
    if (BufferLayout::AOS == out_buffer[i].info.layout) {
      vec.insert(vec.end(),
                 {
                     {PreprocessorCmd::DEFINE, "AOS_LAYOUT"},

                     {PreprocessorCmd::INCLUDE,
                      "\"" + out_buffer[i].info.glsl_filename + "\""},
                     {PreprocessorCmd::UNDEFINE, "AOS_LAYOUT"},

                 });

      layout = layout + "AOS";
    } else {
      vec.insert(vec.end(),
                 {
                     {PreprocessorCmd::INCLUDE,

                      "\"" + out_buffer[i].info.glsl_filename + "\""},
                 });

      layout = layout + "SOA";
    }
    vec.insert(
        vec.end(),
        {
            {PreprocessorCmd::DEFINE, output + " " + out_buffer[i].name},
            {PreprocessorCmd::DEFINE, output + "_VAR " + out_buffer[i].var},
            {PreprocessorCmd::DEFINE,
             std::string(output + "_SIZE ") +
                 std::to_string(out_buffer[i].bufferSize)},
            {PreprocessorCmd::DEFINE,
             std::string(output + "_VAR_SIZE ") +
                 std::to_string(out_buffer[i].varSize)},

            {PreprocessorCmd::DEFINE,
             std::string(output + "_NUM_BUFFER ") +
                 std::to_string(out_buffer[i].bufferNum)},

            {PreprocessorCmd::DEFINE,
             std::string(output + "_INDEX_BUFFER ") +
                 std::to_string(out_buffer[i].bufferIndex)},

            {PreprocessorCmd::DEFINE, layout},
        });
  }

  // return combined commands
  return vec;
}
std::vector<Shader::CommandType> IOBufferData::generateCommands() {
  return generateCommands(false);
}

