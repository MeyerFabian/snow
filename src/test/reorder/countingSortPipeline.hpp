#ifndef COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#define COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#include "../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../src/snow/buffer/c_scan.hpp"

#include "../../src/snow/buffer/buffer.hpp"

#include "../../src/snow/utils/benchmarker.hpp"
#include "../../test/binning/binTechnique.hpp"
#include "../../test/reorder/reorderTechnique.hpp"
#include "../../test/scan/ScanPipeline.hpp"
#include "../BufferData.hpp"
#include "../IOBufferData.hpp"
#include "../SortedBufferData.hpp"
#include "../SortedBufferDataAccess.hpp"
#include "../SortedFullBufferData.hpp"
#include "../SortedIndexReadBufferData.hpp"
#include "../SortedIndexWriteBufferData.hpp"
#include "../map/mapTechnique.hpp"

#define SCAN_DIRECT_WRITE_BACK

class CountingSortPipeline {
 public:
  struct CountingSortData {
    BufferLayout layout;
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };
  struct CountingSortDispatch {
    GLuint numParticles;
    GLuint numGridPoints;
  };

  void run(CountingSortDispatch&&);

  void initIndexReadSort(CountingSortData&& cnt_srt_data,
                         IOBufferData&& io_data);
  void initIndexWriteSort(CountingSortData&& cnt_srt_data,
                          IOBufferData&& io_data);
  // requires a double buffer for the buffer to be sorted
  void initFullSort(CountingSortData&& cnt_srt_data, IOBufferData&& io_data);

  std::vector<std::unique_ptr<SortedBufferData> > getSortedBufferData();

  std::vector<std::unique_ptr<SortedBufferDataAccess> >
  getSortedBufferDataAccess();

 private:
  SortedBufferData::IndexUBOData initUBO(std::string name) const;
  void init(CountingSortData&& cnt_srt_data);
  BufferData initIndexSort(CountingSortData cnt_srt_data, GLuint, GLuint);
  void initUniformBuffer();
  std::unique_ptr<Buffer<GLuint> > binning_buffer;
  std::unique_ptr<Buffer<GLuint> > gridOffsets_buffer;
  std::unique_ptr<Buffer<Scan> > scan_buffer;
  // double buffer, only used for indexSort
  std::unique_ptr<Buffer<GLuint> > index_buffer;
  std::unique_ptr<Buffer<GLuint> > uniform_buffer_sorted;

  MapTechnique resetCounter;
  BinningTechnique binCount;
  ScanPipeline scanPipeline;
  ReorderTechnique reordering;
  std::unique_ptr<BufferData> Scan_local_i;
  std::unique_ptr<BufferData> counter_i;
  std::vector<std::unique_ptr<SortedBufferData> > sorting_data;
  std::vector<std::unique_ptr<BufferDataInterface> > unsorting_data;
  std::deque<GLuint> circle_buffer_values;
};
#endif /* end of include guard: COUNTINGSORTPIPELINE_HPP_W5WCXFCO */

