#ifndef BUFFERDATAINTERFACE_HPP_VGWA5LKH
#define BUFFERDATAINTERFACE_HPP_VGWA5LKH
#include <glm/glm.hpp>
#include <memory>
#include "../../shader/shader.hpp"
/*
 * TODO: make double/triple buffer a child of the interface
 *
 * This interface is used to decouple buffer information from shaders. In the
 * generation of a shader buffer information will be written into it. This
 * allows to write more general shaders.
 *
 * This is the minimal interface to get a hold of or change information:
 */
class BufferDataInterface {
 public:
  virtual ~BufferDataInterface() = default;

  // Generates the preprocessor commands
  virtual std::vector<Shader::CommandType> generateCommands(bool,
                                                            std::string) = 0;

  // name of the buffer
  virtual void setName(std::string) = 0;
  virtual std::string getName() = 0;

  // variable that will be targeted by a shader that uses this option,
  // e.g. map/mapReduce
  virtual void setVariable(std::string) = 0;
  virtual std::string getVariable() = 0;

  // the index to target in a double/triple buffer else 0
  virtual void setIndexBuffer(std::string) = 0;
  virtual std::string getIndexBuffer() = 0;

  // number of elements in the buffer
  virtual GLuint getSize() = 0;

  // clone the interface if other shaders need this information as well
  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface() = 0;
};

#endif /* end of include guard: BUFFERDATAINTERFACE_HPP_VGWA5LKH */

