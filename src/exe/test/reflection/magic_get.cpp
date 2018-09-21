#include <algorithm>
#include <boost/pfr/precise.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include "glm/gtx/string_cast.hpp"
struct Particle {
  glm::vec4 x;
  glm::vec4 v;
  float k;
  float l;
  float m;
  float n;
};
constexpr size_t array_size = 2;
struct Particle_SOA {
  glm::vec4 x[array_size];
  glm::vec4 v[array_size];
  float k[array_size];
  float l[array_size];
  float m[array_size];
  float n[array_size];
};

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
int main() {
  Particle vec[array_size] = {
      {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, 9.0, 10.0, 11.0, 12.0},
      {{13.0, 14.0, 15.0, 16.0},
       {17.0, 18.0, 19.0, 20.0},
       21.0,
       22.0,
       23.0,
       24.0},
  };
  Particle_SOA* out_ptr = new Particle_SOA();
  if (!std::empty(vec)) {
    size_t maxElems = std::size(vec);
    size_t elem_idx = 0;
    for (auto elem_it = std::begin(vec); elem_it < std::end(vec);
         elem_it++, elem_idx++) {
      size_t bytes_offset = 0;
      boost::pfr::for_each_field(
          *elem_it, [&elem_idx, maxElems, &bytes_offset, out_ptr = out_ptr](
                        const auto& field, std::size_t idx) {
            size_t byte_of_field = sizeof(field);

            std::cout << "out_ptr_type" << typeid(out_ptr).name() << std::endl;
            std::cout << "idx: " << idx << " size of type: "
                      << byte_of_field  //<< " value: " << field
                      << " bytes_offset " << bytes_offset << " soa offset:"
                      << elem_idx + bytes_offset / byte_of_field
                      << "\ttype: " << typeid(field).name() << std::endl;

            auto ptr = (std::decay_t<decltype(field)>*)out_ptr;
            ptr[elem_idx + bytes_offset / byte_of_field] = field;

            bytes_offset += maxElems * byte_of_field;
          });
    }
    for (size_t i = 0; i < array_size; i++) {
      std::cout << std::endl;
      boost::pfr::for_each_field(*out_ptr,
                                 [i](const auto& field) { print(field[i]); });
    }
  }
}

