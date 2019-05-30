#ifndef COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#define COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#include "../../../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../../buffer/c_scan.hpp"

#include "../../../buffer/buffer.hpp"

#include "../../../buffer/BufferData/BufferData.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/BufferData/SortedBufferData.hpp"
#include "../../../buffer/BufferData/SortedBufferDataAccess.hpp"
#include "../../../buffer/BufferData/SortedFullBufferData.hpp"
#include "../../../buffer/BufferData/SortedIndexReadBufferData.hpp"
#include "../../../buffer/BufferData/SortedIndexWriteBufferData.hpp"
#include "../../../utils/benchmarker.hpp"
#include "../../computingTechnique/binning/binTechnique.hpp"
#include "../../computingTechnique/reorder/reorderTechnique.hpp"
#include "../../computingTechnique/scan/ScanPipeline.hpp"
#include "../map/mapTechnique.hpp"

#define SCAN_DIRECT_WRITE_BACK

/*
 * Automates executing a number of pipelines and techniques to sort a buffer
 * with histogram/counting sort.
 */
class CountingSortPipeline {
 public:
  struct CountingSortData {
    BufferLayout layout;
    GLuint numGridPoints;
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

  std::unique_ptr<BufferDataInterface> getGridCounter();

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
  std::vector<std::unique_ptr<SortedBufferData> > sorted_data;
  std::vector<std::unique_ptr<BufferDataInterface> > unsorted_data;
  std::deque<GLuint> circle_buffer_values;
};
#endif /* end of include guard: COUNTINGSORTPIPELINE_HPP_W5WCXFCO */

