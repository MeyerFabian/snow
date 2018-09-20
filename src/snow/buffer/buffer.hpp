#ifndef BUFFER_H
#define BUFFER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/pfr/precise.hpp>
#include <iostream>
#include <iterator>
#include <typeindex>
#include <typeinfo>
#include <vector>
enum class BufferType { SSBO };
enum class BufferUsage { STATIC_DRAW, DYNAMIC_DRAW, STATIC_READ, DYNAMIC_READ };
enum class BufferLayout { AOS, SOA };
template <typename ElemT>
class Buffer {
 public:
  Buffer(BufferType t, BufferUsage us, BufferLayout ly)
      : type(t), usage(us), layout(ly), maxElems(0) {
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
#ifdef REFLECTION
    boost::pfr::for_each_field(c[0], [](const auto& field, std::size_t idx) {
      std::cout << idx << ": " << sizeof(field) << '\n';
    });
#endif
    auto ptr = gl_map_buffer(bitmask);
    if (layout == BufferLayout::AOS) {
      auto ElemT_ptr = (ElemT*)ptr;
      size_t i = 0;
      for (const auto& elem : c) {
        ElemT_ptr[i] = elem;
        i++;
      }
    } else {
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
    std::vector<ElemT> data;
    auto ptr = gl_map_buffer(bitmask);
    if (layout == BufferLayout::AOS) {
      auto ElemT_ptr = (ElemT*)ptr;
      data = std::vector<ElemT>(ElemT_ptr, ElemT_ptr + first_n_elem);
    }
    gl_unmap();
    return data;
  }

  auto gl_map_buffer(GLbitfield bitmask) const {
    auto gl_type = gl_map_type();
    return (glMapBufferRange(gl_type, 0, sizeof(ElemT) * (maxElems), bitmask));
  }

  void gl_unmap() const { glUnmapBuffer(gl_map_type()); }
  void gl_bind() const { glBindBuffer(gl_map_type(), bufferHandle); }

  size_t maxElems;

  GLuint bufferHandle;

  GLuint bufferBase;

  BufferType type;
  BufferUsage usage;
  BufferLayout layout;
};

#endif

