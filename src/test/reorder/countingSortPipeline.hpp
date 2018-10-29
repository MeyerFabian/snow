#ifndef COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#define COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#include "../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../src/snow/buffer/c_scan.hpp"

#include "../BufferData.hpp"
#include "../IOBufferData.hpp"
#include "../SortedBufferData.hpp"
#include "../map/mapTechnique.hpp"

class CountingSortPipeline {
 public:
  struct CountingSortData {
    std::string name;
    BufferLayout layout;
    glm::uvec3 gGridDim;
    PREC_VEC3_TYPE gGridPos;
    PREC_SCAL_TYPE gridSpacing;
  };

  void init(CountingSortData&& cnt_srt_data, IOBufferData&& io_data);

 private:
  void initIndexSort(CountingSortData&& cnt_srt_data, IOBufferData&& io_data);
  // requires a double buffer for the buffer to be sorted
  void initFullSort(CountingSortData&& cnt_srt_data, IOBufferData&& io_data);

  std::unique_ptr<Buffer<GLuint> > binning_buffer;
  std::unique_ptr<Buffer<GLuint> > gridOffsets_buffer;
  std::unique_ptr<Buffer<Scan> > scan_buffer;
  // double buffer, only used for indexSort
  std::unique_ptr<Buffer<GLuint> > index_buffer;

  MapTechnique resetCounter;
};
#endif /* end of include guard: COUNTINGSORTPIPELINE_HPP_W5WCXFCO */

