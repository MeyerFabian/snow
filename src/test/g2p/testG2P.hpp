#ifndef TESTG2P_HPP_AVDU9SXE
#define TESTG2P_HPP_AVDU9SXE
#include "../../snow/utils/benchmarker.hpp"
#include "../BlockBufferData.hpp"
#include "../BlockBufferDataAccess.hpp"
#include "../OutputBufferData.hpp"
#include "../SortedBufferData.hpp"
#include "../SortedBufferDataAccess.hpp"
#include "../g2p/g2p_global.hpp"
class TestG2P {
 public:
  struct TestG2PData {
    std::vector<std::unique_ptr<SortedBufferData> > sorted_buffer;
    std::vector<std::unique_ptr<SortedBufferDataAccess> > sorted_buffer_access;
    std::vector<std::unique_ptr<BlockBufferData> > block_buffer;
    std::vector<std::unique_ptr<BlockBufferDataAccess> > block_buffer_access;
    glm::uvec3 gGridDim;
    std::optional<std::shared_ptr<IndirectDispatch> > indirect_dispatch;
  };

  TestG2P(TestG2PData&& tg, OutputBufferData bd) {
    /**********************************************************************
     *                               g2p_io                               *
     **********************************************************************/

    IOBufferData g2p_io;

    g2p_io.in_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));

    g2p_io.out_buffer.push_back(
        std::make_unique<BufferData>(bd.Particle_pos_unsorted));

    G2P_global::G2PData g2p_data{};

    g2pTransfer.init_looping(std::move(g2p_data), std::move(g2p_io));
  }

  void run(GLuint numGridPoints, GLuint numParticles) {
    BenchmarkerGPU::getInstance().time(
        "g2pTransfer_global", [this, &numParticles]() {
          g2pTransfer.dispatch_with_barrier({numParticles});
        });
  };

 private:
  G2P_global g2pTransfer;
};
#endif /* end of include guard: TESTG2P_HPP_AVDU9SXE */

