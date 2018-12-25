#include "countingSortPipeline.hpp"

void CountingSortPipeline::init(CountingSortData&& cnt_srt_data) {
  GLuint gridSize = cnt_srt_data.numGridPoints;

  GLuint to_sort_size = sorting_data[0]->getSize();

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/

  // binning    grid
  binning_buffer = std::make_unique<Buffer<GLuint> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, cnt_srt_data.layout,
      "shader/buffers/atomic_counter.include.glsl");
  binning_buffer->resize_buffer(gridSize);
  binning_buffer->gl_bind_base(ATOMIC_COUNTER_BINDING);

  // gridOffset to_sort
  gridOffsets_buffer = std::make_unique<Buffer<GLuint> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, cnt_srt_data.layout,
      "shader/buffers/particle_grid_offset.include.glsl");
  gridOffsets_buffer->resize_buffer(to_sort_size);
  gridOffsets_buffer->gl_bind_base(PARTICLE_GRIDOFFSET_BINDING);

  // scan       grid
  scan_buffer = std::make_unique<Buffer<Scan> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, cnt_srt_data.layout,
      "shader/buffers/scan_buffer.include.glsl");
  scan_buffer->resize_buffer(gridSize);
  scan_buffer->gl_bind_base(SCAN_BUFFER_BINDING);

  /**********************************************************************
   *                        In/Outs for Shaders                         *
   **********************************************************************/
  // binning     grid
  counter_i = std::make_unique<BufferData>(
      "counters", "Counter_i", binning_buffer->get_buffer_info(), gridSize);

  // gridOffset to_sort
  auto gridOffset_i = BufferData(
      "gridOffsets", "GridOffset_i", gridOffsets_buffer->get_buffer_info(),
      to_sort_size, 1, "0", "Particle_GridOffset_VAR_SIZE");

  // scan       grid
  Scan_local_i = std::make_unique<BufferData>(
      "scans", "Scan_local_i", scan_buffer->get_buffer_info(), gridSize, 1, "0",
      "Scan_VAR_SIZE");

  auto Scan_block_i =
      BufferData("scans", "Scan_block_i", scan_buffer->get_buffer_info(),
                 gridSize, 1, "0", "Scan_VAR_SIZE");
  /**********************************************************************
   *                         Technique/Shaders                          *
   **********************************************************************/
  // Reset
  MapTechnique::MapData map_data{
      "shader/compute/mapreduce/mapSingle.glsl",
      // unary_op
      "0",
  };

  IOBufferData io_map;
  // INPUT
  io_map.in_buffer.push_back(counter_i->cloneBufferDataInterface());
  // OUTPUT
  io_map.out_buffer.push_back(counter_i->cloneBufferDataInterface());

  resetCounter.init(std::move(map_data), std::move(io_map));
  BinningTechnique::BinningData binning_data{
#ifdef BIN_MULTIPLE_ELEMENTS
      "shader/compute/preprocess/bin_mult.glsl",
#else
      "shader/compute/preprocess/bin.glsl",
#endif

#ifdef BIN_MULTIPLE_ELEMENTS
      true,
      BIN_MULTIPLE_ELEMENTS,
#endif
  };
  IOBufferData io_bin;
  // INPUT
  io_bin.in_buffer.push_back(unsorting_data[0]->cloneBufferDataInterface());

  // OUTPUT
  io_bin.out_buffer.push_back(counter_i->cloneBufferDataInterface());

  // OUTPUT2
  io_bin.out_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));

  binCount.init(std::move(binning_data), std::move(io_bin));

  // scan

  ScanTechnique::ScanData scan_data{
      // local_size
      {1024, 1, 1},
      // shader
      "shader/compute/preprocess/scan_unroll.glsl",
      // unary_op_return_type
      "uint",
      // unary_op
      "value",
      // gl_binary_op_neutral_elem
      "0",
      // gl_biunary_op
      "left+right",
      gridSize,
      // raking
      2,
  };

  IOBufferData io_scan;

  // INPUT
  io_scan.in_buffer.push_back(counter_i->cloneBufferDataInterface());

  // OUTPUT
  io_scan.out_buffer.push_back(Scan_local_i->cloneBufferDataInterface());

  // OUTPUT2
  io_scan.out_buffer.push_back(std::make_unique<BufferData>(Scan_block_i));

  scanPipeline = ScanPipeline();
#ifdef SCAN_DIRECT_WRITE_BACK
  scanPipeline.initDirectWriteBack(std::move(scan_data), std::move(io_scan));
#else
  scanPipeline.init(std::move(scan_data), std::move(io_scan));
#endif

  ReorderTechnique::ReorderData reorder_data{
      // LocalSize local_size;
      {32, 1, 1},
      // std::string filename;
      "shader/compute/preprocess/reorder.glsl",
  // GLuint scan_block_size;
#ifdef SCAN_DIRECT_WRITE_BACK
      true,
#else
      false,
      scanPipeline.get_scan_block_size(),
#endif
  };

  IOBufferData io_reorder;

  // INPUT
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));
  // INPUT2
  io_reorder.in_buffer.push_back(Scan_local_i->cloneBufferDataInterface());
  // INPUT3
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(Scan_block_i));
  for (auto it = unsorting_data.begin(); it != unsorting_data.end(); it++) {
    // INPUT4+X
    io_reorder.in_buffer.push_back((*it)->cloneBufferDataInterface());
  }

  for (auto it = sorting_data.begin(); it != sorting_data.end(); it++) {
    // OUTPUT+X
    io_reorder.out_buffer.push_back((*it)->cloneBufferDataInterface());
  }

  reordering = ReorderTechnique();
  reordering.init(std::move(reorder_data), std::move(io_reorder));
}
void CountingSortPipeline::run(CountingSortDispatch&& dispatch_data) {
  if (!circle_buffer_values.empty()) {
    auto circle_buffer_back = circle_buffer_values.back();
    circle_buffer_values.pop_back();
    circle_buffer_values.push_front(circle_buffer_back);
    uniform_buffer_sorted->transfer_to_gpu(circle_buffer_values);
  }

  // reset
  //
  BenchmarkerGPU::getInstance().time(
      "resetCounter", [this, numGridPoints = dispatch_data.numGridPoints]() {
        resetCounter.dispatch_with_barrier({numGridPoints});
      });

  // bin
  BenchmarkerGPU::getInstance().time(
      "Counter", [this, numParticles = dispatch_data.numParticles]() {
        binCount.dispatch_with_barrier(numParticles);
      });
  // scan
  scanPipeline.run(dispatch_data.numGridPoints);
  // reorder
  BenchmarkerGPU::getInstance().time(
      "Reorder Particles", [this, numParticles = dispatch_data.numParticles]() {
        reordering.dispatch_with_barrier({numParticles});
      });
}
SortedBufferData::IndexUBOData CountingSortPipeline::initUBO(
    std::string name) const {
  return SortedBufferData::IndexUBOData{
      "shader/buffers/ubo_sort_unsort.include.glsl",
      name,
  };
}

void CountingSortPipeline::initFullSort(CountingSortData&& cnt_srt_data,
                                        IOBufferData&& io_data) {
  std::string name = io_data.out_buffer[0]->getName();
  for (auto it = io_data.out_buffer.begin(); it != io_data.out_buffer.end();
       it++) {
    auto sorted_buffer = std::make_unique<SortedFullBufferData>(
        (*it)->cloneBufferDataInterface(), initUBO(name));
    sorted_buffer->setIndexBuffer(name + "_sorted");
    auto unsorted_buffer = sorted_buffer->cloneBufferDataInterface();
    unsorted_buffer->setIndexBuffer(name + "_unsorted");
    sorting_data.push_back(std::move(sorted_buffer));
    unsorting_data.push_back(std::move(unsorted_buffer));
  }

  initUniformBuffer();

  init(std::move(cnt_srt_data));
}

BufferData CountingSortPipeline::initIndexSort(CountingSortData cnt_srt_data,
                                               GLuint to_sort_size,
                                               GLuint multiple_buffers) {
  index_buffer = std::make_unique<Buffer<GLuint> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, BufferLayout::SOA,
      "shader/buffers/indices.include.glsl");
  index_buffer->resize_buffer(to_sort_size * multiple_buffers);
  index_buffer->gl_bind_base(PARTICLE_INDICES_BINDING);
  return BufferData("indices", "Index_i", index_buffer->get_buffer_info(),
                    to_sort_size, multiple_buffers, "0", "Indices_VAR_SIZE");
}
void CountingSortPipeline::initIndexReadSort(CountingSortData&& cnt_srt_data,
                                             IOBufferData&& io_data) {
  GLuint to_sort_size = io_data.out_buffer[0]->getSize();
  auto indices_buffer = initIndexSort(cnt_srt_data, to_sort_size, 1);
  for (auto it = io_data.out_buffer.begin(); it != io_data.out_buffer.end();
       it++) {
    auto sorted_buffer = std::make_unique<SortedIndexReadBufferData>(
        (*it)->cloneBufferDataInterface(),
        SortedBufferData::IndexSSBOData{indices_buffer});
    auto unsorted_buffer = sorted_buffer->cloneBufferDataInterface();
    unsorting_data.push_back(std::move(unsorted_buffer));

    sorting_data.push_back(std::move(sorted_buffer));
  }

  init(std::move(cnt_srt_data));
}
void CountingSortPipeline::initUniformBuffer() {
  circle_buffer_values = {0, 1};
  uniform_buffer_sorted = std::make_unique<Buffer<GLuint> >(
      BufferType::UNIFORM, BufferUsage::DYNAMIC_DRAW, BufferLayout::AOS);

  uniform_buffer_sorted->transfer_to_gpu(circle_buffer_values);
  uniform_buffer_sorted->gl_bind_base(UNIFORM_SORTED_BINDING);
}

void CountingSortPipeline::initIndexWriteSort(CountingSortData&& cnt_srt_data,
                                              IOBufferData&& io_data) {
  GLuint to_sort_size = io_data.out_buffer[0]->getSize();
  std::string name = io_data.out_buffer[0]->getName();
  auto indices_buffer_sorted = initIndexSort(cnt_srt_data, to_sort_size, 2);
  auto indices_buffer_unsorted = indices_buffer_sorted;
  indices_buffer_sorted.setIndexBuffer(name + "_sorted");
  indices_buffer_unsorted.setIndexBuffer(name + "_unsorted");
  for (auto it = io_data.out_buffer.begin(); it != io_data.out_buffer.end();
       it++) {
    auto sorted_buffer = std::make_unique<SortedIndexWriteBufferData>(
        (*it)->cloneBufferDataInterface(),
        SortedBufferData::IndexSSBOData{indices_buffer_sorted}, initUBO(name));
    auto unsorted_buffer = std::make_unique<SortedIndexWriteBufferData>(
        (*it)->cloneBufferDataInterface(),
        SortedBufferData::IndexSSBOData{indices_buffer_unsorted},
        initUBO(name));

    sorting_data.push_back(std::move(sorted_buffer));
    unsorting_data.push_back(std::move(unsorted_buffer));
  }

  std::vector<GLuint> index_init;
  for (auto i = 0; i < to_sort_size; i++) {
    index_init.push_back(i);
  }
  index_buffer->transfer_to_gpu(index_init);
  initUniformBuffer();
  init(std::move(cnt_srt_data));
}
std::vector<std::unique_ptr<SortedBufferData> >
CountingSortPipeline::getSortedBufferData() {
  std::vector<std::unique_ptr<SortedBufferData> > ret;
  for (auto& buffer_data : sorting_data) {
    ret.push_back(buffer_data->clone());
  }
  return ret;
}

std::unique_ptr<BufferDataInterface> CountingSortPipeline::getGridCounter() {
  return counter_i->cloneBufferDataInterface();
}

std::vector<std::unique_ptr<SortedBufferDataAccess> >
CountingSortPipeline::getSortedBufferDataAccess() {
  std::vector<std::unique_ptr<SortedBufferDataAccess> > ret;
  for (auto& buffer_data : sorting_data) {
    ret.push_back(std::make_unique<SortedBufferDataAccess>(
        buffer_data->clone(), SortedBufferDataAccess::IndexSSBOData{
                                  Scan_local_i->cloneBufferDataInterface(),
                                  counter_i->cloneBufferDataInterface(),
                              }));
  }
  return ret;
}

