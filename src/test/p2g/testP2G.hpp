#ifndef TESTP2G_HPP_R3XPYRBH
#define TESTP2G_HPP_R3XPYRBH
#include <optional>
#include "../OutputBufferData.hpp"
#include "../map/mapTechnique.hpp"

#include "../../snow/utils/benchmarker.hpp"
#include "../BlockBufferData.hpp"
#include "../BlockBufferDataAccess.hpp"
#include "../SortedBufferData.hpp"
#include "../SortedBufferDataAccess.hpp"
#include "../p2g/p2g_atomic_global.hpp"
#include "../p2g/p2g_shared.hpp"

class TestP2G {
 public:
  struct TestP2GData {
    std::vector<std::unique_ptr<SortedBufferData> > sorted_buffer;
    std::vector<std::unique_ptr<SortedBufferDataAccess> > sorted_buffer_access;
    std::vector<std::unique_ptr<BlockBufferData> > block_buffer;
    std::vector<std::unique_ptr<BlockBufferDataAccess> > block_buffer_access;
    glm::uvec3 gGridDim;
    std::optional<std::shared_ptr<IndirectDispatch> > indirect_dispatch;
  };

  TestP2G(TestP2GData&& tp, OutputBufferData bd) {
    resetGridVel = MapTechnique();
    MapTechnique::MapData map_data{
        "shader/compute/mapreduce/mapSingle.glsl",
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

    IOBufferData p2g_io;
#ifdef FULL_SORTED
#ifdef SHARED
    auto particles_sorted = std::move(tp.sorted_buffer_access);
#else   // NOT SHARED
    auto particles_sorted = std::move(tp.sorted_buffer);
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

#ifdef BLOCK_COMPACTION
#if defined(PUSH_SYNC) || defined(SHARED_PULL)
    auto grid_block_compact = std::move(tp.block_buffer_access);
#else
    auto grid_block_compact = std::move(tp.block_buffer);
#endif
    for (auto& block_buffer_data : grid_block_compact) {
      p2g_io.out_buffer.push_back(
          block_buffer_data->cloneBufferDataInterface());
    }
#else
    p2g_io.out_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));
#endif

    /**********************************************************************
     *                               p2g technique                        *
     **********************************************************************/
#if defined(SHARED)
#if defined(SHARED_ATOMIC_LOOP_REV)  // 2

    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
    };
    p2gTransfer.init_atomic_loop_reverse(std::move(p2g_data),
                                         std::move(p2g_io));
#elif defined(SHARED_ATOMIC)  // 2

#if defined(SHARED_BATCHING_MULT_PART)  // 3
    P2G_shared::P2GBatchingData p2g_data{
        tp.gGridDim,
#ifdef BLOCK_COMPACTION                 // 4
        tp.indirect_dispatch,
#else                                   // NOT BLOCK_COMPACT
        std::nullopt,
#endif                                  // BLOCK_COMPACT 4
        SHARED_BATCHING_MULT_PART,
    };
    p2gTransfer.init_atomic_batching(std::move(p2g_data), std::move(p2g_io));
#else                                   // NO SHARED_BATCHING
    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
#ifdef BLOCK_COMPACTION                 // 4
        tp.indirect_dispatch,
#endif                                  // BLOCK_COMPACTION 4
    };
    p2gTransfer.init_atomic(std::move(p2g_data), std::move(p2g_io));
#endif                                  // SHARED_BATCHING 3

#elif defined(PUSH_SYNC)  // 2

#if defined(SHARED_BATCHING_MULT_PART)  // 3
    P2G_shared::P2GBatchingData p2g_data{
        tp.gGridDim,
        tp.indirect_dispatch,
        SHARED_BATCHING_MULT_PART,
    };
    p2gTransfer.init_sync_batching(std::move(p2g_data), std::move(p2g_io));
#else                                   // NO_SHARED_BATCHING
    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
        tp.indirect_dispatch,
    };
    p2gTransfer.init_sync(std::move(p2g_data), std::move(p2g_io));

#endif  // SHARED_BATCHING 3

#elif defined(SHARED_PULL)  // 1

#if defined(SHARED_BATCHING_MULT_PART)  // 3
    P2G_shared::P2GBatchingData p2g_data{
        tp.gGridDim,
        tp.indirect_dispatch,
        SHARED_BATCHING_MULT_PART,
    };
    p2gTransfer.init_pull_multiple(std::move(p2g_data), std::move(p2g_io));
#else
    P2G_shared::P2GData p2g_data{
        tp.gGridDim,
        tp.indirect_dispatch,
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
#if defined(SHARED)
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

#if defined(SHARED)  // 1
  P2G_shared p2gTransfer;
#else
  P2G_atomic_global p2gTransfer;
#endif
};

#endif /* end of include guard: TESTP2G_HPP_R3XPYRBH */

