#ifndef BUFFER_H
#define BUFFER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <iterator>
#include <vector>

enum class BufferType { SSBO };
enum class BufferUsage { STATIC_DRAW, DYNAMIC_DRAW, STATIC_READ, DYNAMIC_READ };
template <typename ElemT>
class Buffer {
 public:
  Buffer(BufferType t, BufferUsage us) : type(t), usage(us), maxElems(0) {
    glGenBuffers(1, &bufferHandle);
  }
  ~Buffer() { glDeleteBuffers(1, &bufferHandle); }

  template <typename Container>
  void transfer_to_gpu(Container&& c) {
    size_t size = std::size(c);

    gl_bind();
    if (maxElems < size) {
      maxElems = size;
      resize_buffer();
    }
    gl_write_buffer(std::forward<Container>(c));
  }

  std::vector<ElemT> transfer_to_cpu(size_t first_n_elem) const {
    gl_bind();
    return gl_read_buffer(first_n_elem);
  }

  void gl_bind_base(GLuint base) {
    bufferBase = base;
    glBindBufferBase(gl_map_type(), base, bufferHandle);
  }
  GLuint get_name() { return bufferBase; }

  void resize_buffer() {
    glBufferData(gl_map_type(), sizeof(ElemT) * (maxElems), NULL,
                 gl_map_usage());
  }

 private:
  template <typename Container>
  void gl_write_buffer(Container&& c) {
    GLbitfield bitmask = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    auto ptr = gl_map_buffer(bitmask, std::size(c));
    size_t i = 0;
    for (const auto& elem : c) {
      ptr[i] = elem;
      i++;
    }

    gl_unmap();
  }
  GLenum gl_map_type() const {
    switch (type) {
      case BufferType::SSBO:
        return GL_SHADER_STORAGE_BUFFER;
        break;
    }
  }
  GLenum gl_map_usage() const {
    switch (usage) {
      case BufferUsage::STATIC_DRAW:
        return GL_STATIC_DRAW;
        break;
      case BufferUsage::DYNAMIC_DRAW:
        return GL_DYNAMIC_DRAW;
        break;
      case BufferUsage::STATIC_READ:
        return GL_STATIC_READ;
        break;
      case BufferUsage::DYNAMIC_READ:
        return GL_DYNAMIC_READ;
        break;
    }
  }

  std::vector<ElemT> gl_read_buffer(size_t first_n_elem) const {
    GLbitfield bitmask = GL_MAP_READ_BIT;
    ElemT* ptr = gl_map_buffer(bitmask, first_n_elem);
    std::vector<ElemT> data(ptr, ptr + first_n_elem);
    gl_unmap();
    return data;
  }

  auto gl_map_buffer(GLbitfield bitmask, size_t first_n_elem) const {
    auto gl_type = gl_map_type();
    return (ElemT*)(glMapBufferRange(gl_type, 0, sizeof(ElemT) * (first_n_elem),
                                     bitmask));
  }

  void gl_unmap() const { glUnmapBuffer(gl_map_type()); }
  void gl_bind() const { glBindBuffer(gl_map_type(), bufferHandle); }

  size_t maxElems;

  GLuint bufferHandle;

  GLuint bufferBase;

  BufferType type;
  BufferUsage usage;
};

#endif

