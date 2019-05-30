#ifndef SORTEDFULLBUFFERDATA_HPP_RIQXSHUS
#define SORTEDFULLBUFFERDATA_HPP_RIQXSHUS

#include "BufferData.hpp"
#include "SortedBufferData.hpp"
/*
 * Holds buffer data for a double buffer of particles. One will hold the most
 * recently sorted particles, the other one will be one time step behind. Thus
 * we always sort from a hopefully already partially sorted buffer.
 *
 * The sorted buffer can be used by shaders to access particle variables in a
 * coalesced and cache-friendly manner.
 */
class SortedFullBufferData : public SortedBufferData {
 public:
  SortedFullBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                       IndexUBOData&& in_ubo)
      : SortedBufferData(std::move(in_buffer)), ubo(std::move(in_ubo)) {}

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::unique_ptr<SortedBufferData> clone() override;

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;
  IndexUBOData ubo;
};

#endif /* end of include guard: SORTEDFULLBUFFERDATA_HPP_RIQXSHUS */

