#ifndef TESTG2P_HPP_AVDU9SXE
#define TESTG2P_HPP_AVDU9SXE
#include "../../snow/utils/benchmarker.hpp"
#include "../BlockBufferData.hpp"
#include "../BlockBufferDataAccess.hpp"
#include "../OutputBufferData.hpp"
#include "../SortedBufferData.hpp"
#include "../SortedBufferDataAccess.hpp"
#include "../g2p/g2p_global.hpp"
#include "../g2p/g2p_shared.hpp"
class TestG2P {
 public:
  struct TestG2PData {
    glm::uvec3 gGridDim;
    std::vector<std::unique_ptr<SortedBufferData> > sorted_buffer = {};
    std::vector<std::unique_ptr<SortedBufferDataAccess> > sorted_buffer_access =
        {};
    std::vector<std::unique_ptr<BlockBufferData> > block_buffer = {};
    std::optional<std::shared_ptr<IndirectDispatch> > indirect_dispatch =
        std::nullopt;
  };

  TestG2P(TestG2PData&& tg, OutputBufferData bd) {
    /**********************************************************************
     *                               g2p_io                               *
     **********************************************************************/

    IOBufferData g2p_io;

#ifdef FULL_SORTED
#ifdef G2P_SHARED
    auto particles_sorted = std::move(tg.sorted_buffer_access);
#else   // NOT SHARED
    auto particles_sorted = std::move(tg.sorted_buffer);
#endif  // SHARED
    for (auto& particle_sorted : particles_sorted) {
      g2p_io.out_buffer.push_back(particle_sorted->cloneBufferDataInterface());
    }
#else   // NOT FULL_SORTED
    g2p_io.out_buffer.push_back(
        std::make_unique<BufferData>(bd.Particle_pos_unsorted));
#endif  // FULL_SORTED

#ifdef BLOCK_COMPACTION
    auto grid_block_compact = std::move(tg.block_buffer);
    for (auto& block_buffer_data : grid_block_compact) {
      g2p_io.in_buffer.push_back(block_buffer_data->cloneBufferDataInterface());
    }
#else
    g2p_io.in_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));
#endif

#if defined(G2P_SHARED)
#if defined(G2P_SHARED_BATCHING_MULT_PART)
    G2P_shared::G2PBatchingData g2p_data{
        G2P_SHARED_BATCHING_MULT_PART,
        tg.gGridDim,
        tg.indirect_dispatch,
    };
    g2pTransfer.init_pull_batching(std::move(g2p_data), std::move(g2p_io));
#else
    G2P_shared::G2PData g2p_data{
        tg.gGridDim,
        tg.indirect_dispatch,
#ifdef APIC
        true,
#endif
    };
    g2pTransfer.init_pull(std::move(g2p_data), std::move(g2p_io));
#endif

#else
    G2P_global::G2PData g2p_data{
#ifdef APIC
        true,
#endif
    };
    g2pTransfer.init_looping(std::move(g2p_data), std::move(g2p_io));
#endif
  }

  void run(GLuint numGridPoints, GLuint numParticles) {
#if defined(G2P_SHARED)
    BenchmarkerGPU::getInstance().time("g2pTransfer_shared", [this]() {
      g2pTransfer.dispatch_with_barrier({});
    });
#else
    BenchmarkerGPU::getInstance().time(
        "g2pTransfer_global", [this, &numParticles]() {
          g2pTransfer.dispatch_with_barrier({numParticles});
        });
#endif
  };

 private:
#if defined(G2P_SHARED)
  G2P_shared g2pTransfer;
#else
  G2P_global g2pTransfer;
#endif
};
#endif /* end of include guard: TESTG2P_HPP_AVDU9SXE */

