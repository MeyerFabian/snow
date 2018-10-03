#ifndef BUFFER_H
#define BUFFER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <boost/pfr/precise.hpp>
#include <iostream>
#include <iterator>
#include <typeinfo>
#include <vector>
#include "../utils/defines.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
template <typename T, typename std::enable_if_t<
                          std::is_arithmetic_v<std::decay_t<T>>, int> = 0>
void print(T t) {
  std::cout << t << std::endl;
}
template <typename T, typename std::enable_if_t<
                          !std::is_arithmetic_v<std::decay_t<T>>, int> = 0>
void print(T t) {
  std::cout << glm::to_string(t) << std::endl;
}

enum class BufferType { SSBO };
enum class BufferUsage { STATIC_DRAW, DYNAMIC_DRAW, STATIC_READ, DYNAMIC_READ };
enum class BufferLayout { AOS, SOA };

struct BufferInfo {
  BufferLayout layout;
  std::string glsl_filename;
};

template <typename ElemT>
class Buffer {
 public:
  Buffer(BufferType t, BufferUsage us, BufferLayout ly, std::string fn)
      : type(t), usage(us), layout(ly), filename(fn), maxElems(0) {
    glGenBuffers(1, &bufferHandle);
  }
  Buffer(BufferType t, BufferUsage us, BufferLayout ly)
      : type(t), usage(us), layout(ly), maxElems(0) {
    glGenBuffers(1, &bufferHandle);
  }
  ~Buffer() { glDeleteBuffers(1, &bufferHandle); }

  BufferInfo get_buffer_info() { return {layout, filename}; }
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

  template <typename Container>
  void transfer_to_gpu_no_refl(Container&& c) {
    size_t size = std::size(c);

    gl_bind();
    if (maxElems < size) {
      maxElems = size;
      resize_buffer();
    }
    gl_write_buffer_no_refl(std::forward<Container>(c));
  }

  std::vector<ElemT> transfer_to_cpu(size_t first_n_elem) const {
    gl_bind();
    return gl_read_buffer(first_n_elem);
  }

  std::vector<ElemT> transfer_to_cpu_no_refl(size_t first_n_elem) const {
    gl_bind();
    return gl_read_buffer_no_refl(first_n_elem);
  }

  void gl_bind_base(GLuint base) {
    bufferBase = base;
    glBindBufferBase(gl_map_type(), base, bufferHandle);
  }

  GLuint get_base() { return bufferBase; }
  void resize_buffer() {
    glBufferData(gl_map_type(), sizeof(ElemT) * (maxElems), NULL,
                 gl_map_usage());
  }
  void resize_buffer(size_t elements) {
    maxElems = elements;
    resize_buffer();
  }

 private:
  template <typename Container>
  void gl_write_aos(Container&& c, void* ptr) {
    std::cout << "aos" << std::endl;
    auto ElemT_ptr = (ElemT*)ptr;
    size_t i = 0;
    for (const auto& elem : c) {
      ElemT_ptr[i] = elem;
      i++;
    }
  }

  template <typename Container>
  void gl_write_soa(Container&& c, void* ptr) {
    std::cout << "soa: " << std::size(c) << std::endl;
    if (!std::empty(c)) {
      size_t elem_idx = 0;
      for (auto elem_it = std::begin(c); elem_it < std::end(c);
           elem_it++, elem_idx++) {
        size_t bytes_offset = 0;
        boost::pfr::for_each_field(
            *elem_it, [&elem_idx, maxElems = maxElems, &bytes_offset,
                       ptr = ptr](const auto& field, std::size_t idx) {
              size_t byte_of_field = sizeof(field);
              if (elem_idx == 0) {
                std::cout << idx << ", " << typeid(field).name() << ": "
                          << byte_of_field << " val:";
                print(field);
              }
              auto casted_ptr = (std::decay_t<decltype(field)>*)ptr;
              casted_ptr[elem_idx + bytes_offset / byte_of_field] = field;

              bytes_offset += maxElems * byte_of_field;
            });
      }
    }
  }
  template <typename Container>
  void gl_write_buffer(Container&& c) {
    GLbitfield bitmask = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    auto ptr = gl_map_buffer(bitmask);
    if (layout == BufferLayout::AOS) {
      gl_write_aos(std::forward<Container>(c), ptr);
    } else if (layout == BufferLayout::SOA) {
      gl_write_soa(std::forward<Container>(c), ptr);
    }

    gl_unmap();
  }

  template <typename Container>
  void gl_write_buffer_no_refl(Container&& c) {
    GLbitfield bitmask = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    auto ptr = gl_map_buffer(bitmask);
    if (layout == BufferLayout::AOS) {
      gl_write_aos(std::forward<Container>(c), ptr);
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
  void gl_read_aos(std::vector<ElemT>& c, void* ptr,
                   size_t first_n_elem) const {
    auto ElemT_ptr = (ElemT*)ptr;
    c = std::vector<ElemT>(ElemT_ptr, ElemT_ptr + first_n_elem);
  }

  void gl_read_soa(std::vector<ElemT>& c, void* ptr,
                   size_t first_n_elem) const {
    for (size_t elem_idx = 0; elem_idx < first_n_elem; elem_idx++) {
      size_t bytes_offset = 0;
      ElemT aos_elem = {};
      boost::pfr::for_each_field(
          aos_elem, [&elem_idx, maxElems = maxElems, &bytes_offset, ptr = ptr](
                        auto& field, std::size_t idx) {
            size_t byte_of_field = sizeof(field);
            auto casted_ptr = (std::decay_t<decltype(field)>*)ptr;
            field = casted_ptr[elem_idx + bytes_offset / byte_of_field];
            if (elem_idx == 0) {
              std::cout << idx << ", " << typeid(field).name() << ": "
                        << byte_of_field << " val: ";
              print(field);
            }

            bytes_offset += maxElems * byte_of_field;
          });
      c.push_back(std::move(aos_elem));
    }
  }

  std::vector<ElemT> gl_read_buffer(size_t first_n_elem) const {
    GLbitfield bitmask = GL_MAP_READ_BIT;
    std::vector<ElemT> data;
    data.reserve(first_n_elem);
    auto ptr = gl_map_buffer(bitmask);
    if (layout == BufferLayout::AOS) {
      gl_read_aos(data, ptr, first_n_elem);
    } else if (layout == BufferLayout::SOA) {
      gl_read_soa(data, ptr, first_n_elem);
    }

    gl_unmap();
    return data;
  }

  std::vector<ElemT> gl_read_buffer_no_refl(size_t first_n_elem) const {
    GLbitfield bitmask = GL_MAP_READ_BIT;
    std::vector<ElemT> data;
    data.reserve(first_n_elem);
    auto ptr = gl_map_buffer(bitmask);
    if (layout == BufferLayout::AOS) {
      gl_read_aos(data, ptr, first_n_elem);
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
  std::string filename;
};

#endif

