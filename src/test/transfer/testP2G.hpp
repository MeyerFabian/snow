#ifndef TESTP2G_HPP_R3XPYRBH
#define TESTP2G_HPP_R3XPYRBH
#include <optional>
#include "../../core/simulation/computingTechnique/map/mapTechnique.hpp"
#include "../OutputBufferData.hpp"

#include "../../core/buffer/BufferData/BlockBufferData.hpp"
#include "../../core/buffer/BufferData/BlockBufferDataAccess.hpp"
#include "../../core/buffer/BufferData/SortedBufferData.hpp"
#include "../../core/buffer/BufferData/SortedBufferDataAccess.hpp"
#include "../../core/simulation/computingTechnique/p2g/p2g_atomic_global.hpp"
#include "../../core/simulation/computingTechnique/p2g/p2g_shared.hpp"
#include "../../core/utils/benchmarker.hpp"

class TestP2G {
 public:
  struct TestP2GData {
    glm::uvec3 gGridDim;
    std::vector<std::unique_ptr<SortedBufferData> > particles_sorted = {};
    std::vector<std::unique_ptr<SortedBufferDataAccess> >
        particles_sorted_access = {};
    std::vector<std::unique_ptr<BlockBufferData> > grid_block = {};
    std::vector<std::unique_ptr<BlockBufferDataAccess> > grid_block_access = {};
    std::optional<std::shared_ptr<IndirectDispatch> > active_block_call =
        std::nullopt;
  };

  TestP2G(TestP2GData&& tp, OutputBufferData bd) {
    // If a global atomic P2G is chosen, data needs to be reset on the grid.
    resetGridVel = MapTechnique();
    MapTechnique::MapData map_data{
        "shader/compute/map/map.glsl",
        // unary_op
        "vec4(0.0,0.0,0.0,0.0)",
    };

    IOBufferData io_map;
    // INPUT
    io_map.in_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));
    // OUTPUT
    io_map.out_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));

    resetGridVel.init(std::move(map_data), std::move(io_map));

    /**********************************************************************
     *                               p2g_io in                            *
     **********************************************************************/

    // Prepare particle BufferData as input
    IOBufferData p2g_io;
#ifdef FULL_SORTED
#ifdef P2G_SHARED
    auto particles_sorted = std::move(tp.particles_sorted_access);
#else   // NOT SHARED
    auto particles_sorted = std::move(tp.particles_sorted);
#endif  // SHARED
    for (auto& particle_sorted : particles_sorted) {
      p2g_io.in_buffer.push_back(particle_sorted->cloneBufferDataInterface());
    }
#else   // NOT FULL_SORTED
    p2g_io.in_buffer.push_back(
        std::make_unique<BufferData>(bd.Particle_pos_unsorted));
#endif  // FULL_SORTED

    /**********************************************************************
     *                               p2g_io out                           *
     **********************************************************************/

    // Prepare grid BufferData as output
#ifdef BLOCK_COMPACTION
#if defined(PUSH_SYNC) || defined(SHARED_PULL)
    auto grid_block_compact = std::move(tp.grid_block_access);
#else
    auto grid_block_compact = std::move(tp.grid_block);
#endif
    for (auto& grid_block_data : grid_block_compact) {
      p2g_io.out_buffer.push_back(grid_block_data->cloneBufferDataInterface());
    }
#else
    p2g_io.out_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));
#endif

    /**********************************************************************
     *                               p2g technique                        *
     **********************************************************************/
    // Prepare the P2G-technique and initialize it
#if defined(P2G_SHARED)
#if defined(SHARED_ATOMIC_LOOP_REV)  // 2

    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
    };
    p2gTransfer.init_atomic_loop_reverse(std::move(p2g_data),
                                         std::move(p2g_io));
#elif defined(SHARED_ATOMIC)  // 2

#if defined(P2G_SHARED_BATCHING_MULT_PART)  // 3
    P2G_shared::P2GBatchingData p2g_data{
        P2G_SHARED_BATCHING_MULT_PART,
        tp.gGridDim,
#ifdef BLOCK_COMPACTION                     // 4
        tp.active_block_call,
#else                                       // NOT BLOCK_COMPACT
        std::nullopt,
#endif                                      // BLOCK_COMPACT 4
    };
    p2gTransfer.init_atomic_batching(std::move(p2g_data), std::move(p2g_io));
#else                                       // NO SHARED_BATCHING
    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
#ifdef BLOCK_COMPACTION                     // 4
        tp.active_block_call,
#endif                                      // BLOCK_COMPACTION 4
    };
    p2gTransfer.init_atomic(std::move(p2g_data), std::move(p2g_io));
#endif                                      // SHARED_BATCHING 3

#elif defined(PUSH_SYNC)  // 2

#if defined(P2G_SHARED_BATCHING_MULT_PART)  // 3
    P2G_shared::P2GBatchingData p2g_data{
        P2G_SHARED_BATCHING_MULT_PART,
        tp.gGridDim,
        tp.active_block_call,
#ifdef APIC
        true,
#endif
    };
    p2gTransfer.init_sync_batching(std::move(p2g_data), std::move(p2g_io));
#else  // NO_SHARED_BATCHING
    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
        tp.active_block_call,
#ifdef APIC
        true,
#endif
    };
    p2gTransfer.init_sync(std::move(p2g_data), std::move(p2g_io));

#endif  // SHARED_BATCHING 3

#elif defined(SHARED_PULL)  // 1

#if defined(P2G_SHARED_BATCHING_MULT_PART)  // 3
    P2G_shared::P2GBatchingData p2g_data{
        P2G_SHARED_BATCHING_MULT_PART,
        tp.gGridDim,
        tp.active_block_call,
    };
    p2gTransfer.init_pull_multiple(std::move(p2g_data), std::move(p2g_io));
#else
    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
        tp.active_block_call,
    };
    p2gTransfer.init_pull_simple(std::move(p2g_data), std::move(p2g_io));
#endif
#endif
#elif defined(ATOMIC_LOOP)  // 1
    P2G_atomic_global::P2GData p2g_data{};

    p2gTransfer.init_looping(std::move(p2g_data), std::move(p2g_io));
#else                       // 1
    // global w/o looping
    P2G_atomic_global::P2GData p2g_data{};
    p2gTransfer.init_too_parallel(std::move(p2g_data), std::move(p2g_io));
#endif
  }

  void run(GLuint numGridPoints, GLuint numParticles) {
    BenchmarkerGPU::getInstance().time(
        "resetGridVel", [this, &numGridPoints]() {
          resetGridVel.dispatch_with_barrier({numGridPoints});
        });
#if defined(P2G_SHARED)
    BenchmarkerGPU::getInstance().time("p2gTransfer_shared", [this]() {
      p2gTransfer.dispatch_with_barrier({});
    });
#else
    BenchmarkerGPU::getInstance().time(
        "p2gTransfer_atomic", [this, &numParticles]() {
          p2gTransfer.dispatch_with_barrier({numParticles});
        });
#endif
  }

 private:
  MapTechnique resetGridVel;

#if defined(P2G_SHARED)  // 1
  P2G_shared p2gTransfer;
#else
  P2G_atomic_global p2gTransfer;
#endif
};

#endif /* end of include guard: TESTP2G_HPP_R3XPYRBH */

