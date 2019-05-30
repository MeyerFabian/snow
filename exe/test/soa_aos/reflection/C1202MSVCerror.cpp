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
struct Particle_1 {
  glm::dvec4 a;
  glm::dvec4 b;
  glm::dvec4 c;
  glm::dvec4 d;
};
struct Particle_2 {
  glm::dvec4 e;
  glm::dvec4 f;
  glm::dvec4 g;
};

struct Particle {
  Particle_1 part_1;
  Particle_2 part_2;
};

int main() {
  Particle particle = {};
  /*
    auto t = std::make_tuple(Particle_1(), Particle_2());
    std::cout << sizeof(std::get<1>(t));
    

      boost::pfr::for_each_field(std::get<1>(t), [](const auto& field,
                                                    std::size_t idx) {
        size_t byte_of_field = sizeof(field);

        std::cout << "idx: " << idx
                  << " size of type: " << byte_of_field  //<< " value: " <<
    field
                  << "\ttype: " << typeid(field).name() << std::endl;
      });
      boost::pfr::for_each_field(t, [](const auto& field, std::size_t ixd) {
        boost::pfr::for_each_field(field, [](const auto& field, std::size_t idx)
    { size_t byte_of_field = sizeof(field);

          std::cout << "idx: " << idx
                    << " size of type: " << byte_of_field  //<< " value: " <<
      field
                    << "\ttype: " << typeid(field).name() << std::endl;
        });
      });
      */
}

