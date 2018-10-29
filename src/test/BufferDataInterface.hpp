#ifndef BUFFERDATAINTERFACE_HPP_VGWA5LKH
#define BUFFERDATAINTERFACE_HPP_VGWA5LKH
#include <glm/glm.hpp>
#include <memory>
#include "../snow/shader/shader.hpp"
class BufferDataInterface {
 public:
  virtual std::vector<Shader::CommandType> generateCommands(bool,
                                                            std::string) = 0;

  virtual void setName(std::string) = 0;
  virtual void setVariable(std::string) = 0;

  virtual std::string getName() = 0;
  virtual std::string getVariable() = 0;
  virtual GLuint getSize() = 0;

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      const = 0;
  virtual ~BufferDataInterface() = default;
};

#endif /* end of include guard: BUFFERDATAINTERFACE_HPP_VGWA5LKH */

