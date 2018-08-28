#ifndef BUFFER_H
#define BUFFER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <iterator>
#include <vector>

enum class BufferType { SSBO };
template <typename ElemT>
class Buffer {
 public:
  Buffer(BufferType t) : type(t), maxElems(0) {
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

  std::vector<ElemT> transfer_to_cpu() {
    gl_bind();
    return gl_read_buffer();
  }

  void gl_bind_base(GLuint base) {
    glBindBufferBase(gl_map_type(), base, bufferHandle);
  }

 private:
  template <typename Container>
  void gl_write_buffer(Container&& c) {
    GLbitfield bitmask = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    auto ptr = gl_map_buffer(bitmask);
    size_t i = 0;
    for (const auto& elem : c) {
      ptr[i] = elem;
      i++;
    }

    gl_unmap();
  }
  GLenum gl_map_type() {
    switch (type) {
      case BufferType::SSBO:
        return GL_SHADER_STORAGE_BUFFER;
        break;
    }
  }
  void resize_buffer() {
    glBufferData(gl_map_type(), sizeof(ElemT) * (maxElems), NULL,
                 GL_STATIC_DRAW);
  }
  std::vector<ElemT> gl_read_buffer() {
    GLbitfield bitmask = GL_MAP_READ_BIT;
    ElemT* ptr = gl_map_buffer(bitmask);
    std::vector<ElemT> data(ptr, ptr + maxElems);
    gl_unmap();
    return data;
  }

  auto gl_map_buffer(GLbitfield bitmask) {
    auto gl_type = gl_map_type();
    return (ElemT*)(glMapBufferRange(gl_type, 0, sizeof(ElemT) * (maxElems),
                                     bitmask));
  }

  void gl_unmap() { glUnmapBuffer(gl_map_type()); }
  void gl_bind() { glBindBuffer(gl_map_type(), bufferHandle); }

  size_t maxElems;

  GLuint bufferHandle;

  BufferType type;
};

#endif

