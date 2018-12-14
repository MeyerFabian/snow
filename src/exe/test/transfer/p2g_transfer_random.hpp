#ifndef ATOMIC_HPP_CN2ZL6DH
#define ATOMIC_HPP_CN2ZL6DH
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "../../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../../test/BufferData.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/p2g/p2g_atomic_global.hpp"
#include "../../../test/reorder/countingSortPipeline.hpp"
#include "../../../test/test_util.hpp"
#include "../../src/snow/grid/gridpoint.hpp"
#include "../../src/snow/particle/particle_exp.hpp"

struct testData {
  GLuint numParticles;
  GLuint numGridPoints;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;
  glm::uvec3 gGridDim;
  PREC_VEC3_TYPE gGridPos;
  PREC_SCAL_TYPE gridSpacing;
};

struct OutputData {
  std::vector<Gridpoint> grid;
};

OutputData test(testData&& data) {
  GLFWWindow();

#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif

  /**********************************************************************
   *                          Buffer creations                          *
   **********************************************************************/

  // Particle_exp
  Buffer<Particle_exp> particle_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_system.include.glsl");

  particle_buffer.transfer_to_gpu(data.particles);
  particle_buffer.gl_bind_base(PARTICLE_SYSTEM_BINDING);
  // Particle_exp2
  Buffer<Particle_exp_2> particle_2_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_system.include.glsl");

  particle_2_buffer.transfer_to_gpu(data.particles2);
  particle_2_buffer.gl_bind_base(PARTICLE_SYSTEM_BINDING_2);

  // Gridpoints
  Buffer<Gridpoint> grid_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW,
                                layout,
                                "shader/buffers/uniform_grid.include.glsl");

  grid_buffer.resize_buffer(data.numGridPoints);
  grid_buffer.gl_bind_base(UNIFORM_GRID_BINDING);

  /**********************************************************************
   *                    Techniques + IOData creation                    *
   **********************************************************************/

#ifdef FULL_SORTED
  // full sort double buffered
  auto Particle_pos_unsorted = BufferData(
      "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
      data.numParticles, 2, "0", "Particle_exp_size");

  auto Particle_2_unsorted =
      BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                 data.numParticles, 2, "0",

                 "Particle_exp_2_size");
#else
  auto Particle_pos_unsorted = BufferData(
      "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
      data.numParticles, 1, "0", "Particle_exp_size");

  auto Particle_2_unsorted =
      BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                 data.numParticles, 1, "0", "Particle_exp_2_size");
#endif

#ifdef SORTED
  CountingSortPipeline::CountingSortData cnt_srt_data{
      layout,
      data.gGridPos,
      data.gGridDim,
      data.gridSpacing,
  };

  CountingSortPipeline cnt_srt_pipeline;
  IOBufferData io_cnt_srt;
  // OUTPUT
  io_cnt_srt.out_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_unsorted));
  io_cnt_srt.out_buffer.push_back(
      std::make_unique<BufferData>(Particle_2_unsorted));

#endif

#if defined(FULL_SORTED)
  cnt_srt_pipeline.initFullSort(std::move(cnt_srt_data), std::move(io_cnt_srt));
#elif defined(INDEX_SORTED)
#if defined(INDEX_WRITE_SORTED)
  cnt_srt_pipeline.initIndexWriteSort(std::move(cnt_srt_data),
                                      std::move(io_cnt_srt));
#else

  cnt_srt_pipeline.initIndexReadSort(std::move(cnt_srt_data),
                                     std::move(io_cnt_srt));
#endif
#endif
  auto gridpoint_vel_mass = BufferData(
      "gridpoints", "Gridpoint_vel_mass", grid_buffer.get_buffer_info(),
      data.numGridPoints, 1, "0", "Gridpoint_size");

  auto resetGridVel = MapTechnique();
  MapTechnique::MapData map_data{
      "shader/compute/mapreduce/mapSingle.glsl",
      // unary_op
      "vec4(0.0,0.0,0.0,0.0)",
  };

  IOBufferData io_map;
  // INPUT
  io_map.in_buffer.push_back(std::make_unique<BufferData>(gridpoint_vel_mass));
  // OUTPUT
  io_map.out_buffer.push_back(std::make_unique<BufferData>(gridpoint_vel_mass));

  resetGridVel.init(std::move(map_data), std::move(io_map));

  auto p2gTransfer = P2G_atomic_global();
  P2G_atomic_global::P2GData p2g_data{
      data.gGridPos,
      data.gGridDim,
      data.gridSpacing,

  };

  IOBufferData p2g_io;
  // INPUT
  p2g_io.in_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_unsorted));

  // OUTPUT
  p2g_io.out_buffer.push_back(std::make_unique<BufferData>(gridpoint_vel_mass));
#ifdef ATOMIC_LOOP

  p2gTransfer.init_looping(std::move(p2g_data), std::move(p2g_io));
#else
  p2gTransfer.init_too_parallel(std::move(p2g_data), std::move(p2g_io));
#endif

  /**********************************************************************
   *                         execute dispatches                         *
   **********************************************************************/

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent",
             [
#ifdef FULL_SORTED
                 &cnt_srt_pipeline,
#endif
                 &resetGridVel, &p2gTransfer, numParticles = data.numParticles,
                 numGridPoints = data.numGridPoints]() {
               executeTest(1, [
#ifdef FULL_SORTED
                                  &cnt_srt_pipeline,
#endif
                                  &resetGridVel, &p2gTransfer, &numParticles,
                                  &numGridPoints]() {
#ifdef FULL_SORTED
                 cnt_srt_pipeline.run({numParticles, numGridPoints});
#endif
                 BenchmarkerGPU::getInstance().time(
                     "resetGridVel", [&resetGridVel, &numGridPoints]() {
                       resetGridVel.dispatch_with_barrier({numGridPoints});
                     });

                 BenchmarkerGPU::getInstance().time(
                     "p2gTransfer", [&p2gTransfer, &numParticles]() {
                       p2gTransfer.dispatch_with_barrier({numParticles});
                     });
               });
             });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("p2gTransfer");
  bench.write_to_file("p2gTransferCPU");
  return {
      grid_buffer.transfer_to_cpu(data.numGridPoints),
  };
}
#endif /* end of include guard: COUNT_SRT_HPP_CN2ZL6DH */

