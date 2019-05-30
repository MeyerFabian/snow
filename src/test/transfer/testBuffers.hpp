#ifndef TESTBUFFERS_HPP_XGJ0L97E
#define TESTBUFFERS_HPP_XGJ0L97E
#include "../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../core/buffer/buffer.hpp"

#include "../../core/buffer/BufferData/BufferData.hpp"
#include "../../core/object/grid/grid_def.hpp"
#include "../../core/object/grid/gridpoint_exp.hpp"
#include "../../core/object/particle/particle_exp.hpp"

#include "testData.hpp"

#include "../OutputBufferData.hpp"
class TestBuffers {
 public:
  TestBuffers(testData&& data, BufferLayout layout)
      :

        grid_def_buffer(BufferType::UNIFORM, BufferUsage::DYNAMIC_DRAW,
                        BufferLayout::AOS),
        grid_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                    "shader/buffers/uniform_grid.include.glsl"),
        // Particle_exp
        particle_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                        "shader/buffers/particle_system.include.glsl"),
        particle_2_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                          "shader/buffers/particle_system.include.glsl")

  {
    /**********************************************************************
     *                          Buffer creations                          *
     **********************************************************************/
    numGridPoints = data.numGridPoints;
    numParticles = data.numParticles;

    grid_def_buffer.transfer_to_gpu(std::vector<GridDefines>{data.grid_def});
    grid_def_buffer.gl_bind_base(GRID_DEFINES_BINDING);

    particle_buffer.transfer_to_gpu(data.particles);
    particle_buffer.gl_bind_base(PARTICLE_SYSTEM_BINDING);

    // Particle_exp2
    particle_2_buffer.transfer_to_gpu(data.particles2);
    particle_2_buffer.gl_bind_base(PARTICLE_SYSTEM_BINDING_2);

    // Gridpoints
    grid_buffer.resize_buffer(data.numGridPoints);
    grid_buffer.gl_bind_base(UNIFORM_GRID_BINDING);
  }

  OutputBufferData getBufferData() {
    /**********************************************************************
     *                    Techniques + IOData creation                    *
     **********************************************************************/
    auto gridpoint_vel_mass = BufferData(
        "gridpoints", "Gridpoint_vel_mass", grid_buffer.get_buffer_info(),
        numGridPoints, 1, "0", "Gridpoint_size");

#ifdef FULL_SORTED
    // full sort double buffered
    auto Particle_pos_unsorted = BufferData(
        "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
        numParticles, 2, "0", "Particle_exp_size");

    auto Particle_2_unsorted =
        BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                   numParticles, 2, "0", "Particle_exp_2_size");
#else
    auto Particle_pos_unsorted = BufferData(
        "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
        numParticles, 1, "0", "Particle_exp_size");

    auto Particle_2_unsorted =
        BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                   numParticles, 1, "0", "Particle_exp_2_size");
#endif
    return OutputBufferData(
        {gridpoint_vel_mass, Particle_pos_unsorted, Particle_2_unsorted});
  }
  std::vector<Particle_exp> getParticles(GLuint numParticles) {
    return particle_buffer.transfer_to_cpu(numParticles);
  }
  std::vector<Particle_exp_2> getParticles2(GLuint numParticles) {
    return particle_2_buffer.transfer_to_cpu(numParticles);
  }
  std::vector<Gridpoint> getGridPoints(GLuint numGridPoints) {
    return grid_buffer.transfer_to_cpu(numGridPoints);
  }

 private:
  Buffer<GridDefines> grid_def_buffer;
  Buffer<Gridpoint> grid_buffer;
  Buffer<Particle_exp> particle_buffer;
  Buffer<Particle_exp_2> particle_2_buffer;
  GLuint numParticles;
  GLuint numGridPoints;
};
#endif /* end of include guard: TESTBUFFERS_HPP_XGJ0L97E */

