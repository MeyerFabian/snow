#include "countingSortPipeline.hpp"

void CountingSortPipeline::init(CountingSortData&& cnt_srt_data) {
  GLuint gridSize = cnt_srt_data.numGridPoints;

  GLuint to_sort_size = sorted_data[0]->getSize();

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
      "shader/compute/map/map.glsl",
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
      "shader/test/sort/bin/bin_mult.glsl",
#else
      "shader/compute/sort/bin/bin.glsl",
#endif

#ifdef BIN_MULTIPLE_ELEMENTS
      true,
      BIN_MULTIPLE_ELEMENTS,
#endif
  };

  IOBufferData io_bin;
  // INPUT to_sort positions
  io_bin.in_buffer.push_back(unsorted_data[0]->cloneBufferDataInterface());

  // OUTPUT per cell to_sort counter
  io_bin.out_buffer.push_back(counter_i->cloneBufferDataInterface());

  // OUTPUT2 per cell to_sort offset
  io_bin.out_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));

  binCount.init(std::move(binning_data), std::move(io_bin));

  // scan

  ScanTechnique::ScanData scan_data{
      // local_size
      {1024, 1, 1},
      // shader
      "shader/compute/scan/scan_unroll.glsl",
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

  // INPUT per cell to_sort counter
  io_scan.in_buffer.push_back(counter_i->cloneBufferDataInterface());

  // OUTPUT a local scan
  io_scan.out_buffer.push_back(Scan_local_i->cloneBufferDataInterface());

  // OUTPUT2 a block scan
  io_scan.out_buffer.push_back(std::make_unique<BufferData>(Scan_block_i));

  scanPipeline = ScanPipeline();
#ifdef SCAN_DIRECT_WRITE_BACK
  // immediately writes back from block to local scan
  scanPipeline.initDirectWriteBack(std::move(scan_data), std::move(io_scan));
#else
  // only does local and block scan and does not write back from block to local
  scanPipeline.init(std::move(scan_data), std::move(io_scan));
#endif

  // reoders according to the scan+offset position
  // this may reorder only indices or do a full deep sort of all variables
  ReorderTechnique::ReorderData reorder_data{
      // LocalSize local_size;
      {32, 1, 1},
      // std::string filename;
      "shader/compute/sort/reorder/reorder.glsl",
  // GLuint scan_block_size;
#ifdef SCAN_DIRECT_WRITE_BACK
      true,
#else
      false,
      scanPipeline.get_scan_block_size(),
#endif
  };

  IOBufferData io_reorder;

  // INPUT per to_sort grid offsets
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));
  // INPUT2 per cell scan
  io_reorder.in_buffer.push_back(Scan_local_i->cloneBufferDataInterface());
  // INPUT3 per cell scan
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(Scan_block_i));
  for (auto it = unsorted_data.begin(); it != unsorted_data.end(); it++) {
    // INPUT4+X are the to_sort
    io_reorder.in_buffer.push_back((*it)->cloneBufferDataInterface());
  }

  for (auto it = sorted_data.begin(); it != sorted_data.end(); it++) {
    // OUTPUT+X is the grid
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
/* Initializes named SortedBufferData for ubo. In use for index write and full
 * sort.
 */
SortedBufferData::IndexUBOData CountingSortPipeline::initUBO(
    std::string name) const {
  return SortedBufferData::IndexUBOData{
      "shader/buffers/ubo_sort_unsort.include.glsl",
      name,
  };
}

/* Initializes a full deep sort reordering any variable in the to be sorted
 * buffer into the output buffer. For ease of use with other shaders i
 * assume input=output buffer and double buffering controlled by a
 * ubo(uniform_buffer_sorted).
 *
 * @io_data.out_buffer[0] : double buffer to sort
 *                    ... : more variables
 */
void CountingSortPipeline::initFullSort(CountingSortData&& cnt_srt_data,
                                        IOBufferData&& io_data) {
  /* Steals name of the buffer to sort s.t. initUBO can initialize a ubo which
   * shows which part of the double buffer is sorted and which has the
   * previously sorted state.
   */
  std::string name = io_data.out_buffer[0]->getName();
  for (auto it = io_data.out_buffer.begin(); it != io_data.out_buffer.end();
       it++) {
    auto sorted_buffer = std::make_unique<SortedFullBufferData>(
        (*it)->cloneBufferDataInterface(), initUBO(name));
    // setIndexBuffer identifies if the front or back is sorted by the ubo
    sorted_buffer->setIndexBuffer(name + "_sorted");
    auto unsorted_buffer = sorted_buffer->cloneBufferDataInterface();
    unsorted_buffer->setIndexBuffer(name + "_unsorted");
    sorted_data.push_back(std::move(sorted_buffer));
    unsorted_data.push_back(std::move(unsorted_buffer));
  }

  initUniformBuffer();

  init(std::move(cnt_srt_data));
}

/*
 * @to_sort_size    : size of the buffer to be sorted so the index buffer holds
 *                    the same amount of elements
 * @multiple_buffers: single-(1),double-buffered(2) for IndexRead and
 *                    IndexWrite, respectively
 */
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

/*
 * Index read always uses the unsorted particles as an input with standard
 * indices and output being the sorted access.
 *
 * e.g.:
 * sorted: 3,1,2,0,4
 * 0,1,2,3,4 -> 3,1,2,0,4
 *
 * @io_data.out_buffer[0] : buffer to create sorted access index buffer for
 *                    ... : more variables
 */
void CountingSortPipeline::initIndexReadSort(CountingSortData&& cnt_srt_data,
                                             IOBufferData&& io_data) {
  GLuint to_sort_size = io_data.out_buffer[0]->getSize();
  // create a new single buffer for indices
  auto indices_buffer = initIndexSort(cnt_srt_data, to_sort_size, 1);
  for (auto it = io_data.out_buffer.begin(); it != io_data.out_buffer.end();
       it++) {
    // BufferData are the same, no difference
    auto sorted_buffer = std::make_unique<SortedIndexReadBufferData>(
        (*it)->cloneBufferDataInterface(),
        SortedBufferData::IndexSSBOData{indices_buffer});
    auto unsorted_buffer = sorted_buffer->cloneBufferDataInterface();
    unsorted_data.push_back(std::move(unsorted_buffer));

    sorted_data.push_back(std::move(sorted_buffer));
  }

  init(std::move(cnt_srt_data));
}

/*
 * Creates a UBO buffer to keep track which part of the double buffer has the
 * newly sorted state. 1 is the buffer to put the newly sorted state.
 */
void CountingSortPipeline::initUniformBuffer() {
  circle_buffer_values = {0, 1};
  uniform_buffer_sorted = std::make_unique<Buffer<GLuint> >(
      BufferType::UNIFORM, BufferUsage::DYNAMIC_DRAW, BufferLayout::AOS);

  uniform_buffer_sorted->transfer_to_gpu(circle_buffer_values);
  uniform_buffer_sorted->gl_bind_base(UNIFORM_SORTED_BINDING);
}

/* Initializes a index write sort putting out indices to access the buffer in a
 * sorted manner. Buffer will not be sorted! It will take the last
 * sorted state to increase coalescing on writes in a double buffer. This double
 * buffer stores to the indexing of the newly sorted and last sorted state.
 *
 * e.g. last sorted state: 3,2,1,0,4
 * sorted state: 3,1,2,0,4
 * 3,2,1,0,4 -> 3,1,2,0,4
 *
 * @io_data.out_buffer[0] : buffer to create sorted access index buffer for
 *                    ... : more variables
 *
 */
void CountingSortPipeline::initIndexWriteSort(CountingSortData&& cnt_srt_data,
                                              IOBufferData&& io_data) {
  GLuint to_sort_size = io_data.out_buffer[0]->getSize();
  std::string name = io_data.out_buffer[0]->getName();
  // creates and names the double buffer
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

    sorted_data.push_back(std::move(sorted_buffer));
    unsorted_data.push_back(std::move(unsorted_buffer));
  }

  // need to initialize the buffer with some kind of indexing, so standard
  // 1,2,3,4 is chosen here
  std::vector<GLuint> index_init;
  for (auto i = 0; i < to_sort_size; i++) {
    index_init.push_back(i);
  }
  index_buffer->transfer_to_gpu(index_init);
  initUniformBuffer();
  init(std::move(cnt_srt_data));
}

/*
 * This only makes sure that the buffer can be accessed sorted due to one of the
 * three possibilites: IndexRead, IndexWrite, FullSort.
 */
std::vector<std::unique_ptr<SortedBufferData> >
CountingSortPipeline::getSortedBufferData() {
  std::vector<std::unique_ptr<SortedBufferData> > ret;
  for (auto& buffer_data : sorted_data) {
    ret.push_back(buffer_data->clone());
  }
  return ret;
}

/*
 * Request the per cell grid counter buffer data. This was created in the
 * pipeline due to binning.
 */
std::unique_ptr<BufferDataInterface> CountingSortPipeline::getGridCounter() {
  return counter_i->cloneBufferDataInterface();
}

/*
 * Makes sure that the buffer can be accessed sorted due to one of the
 * three possibilites: IndexRead, IndexWrite, FullSort. Additionally the scan
 * and counter (from binning) are also made accessible for other shaders.
 */
std::vector<std::unique_ptr<SortedBufferDataAccess> >
CountingSortPipeline::getSortedBufferDataAccess() {
  std::vector<std::unique_ptr<SortedBufferDataAccess> > ret;
  for (auto& buffer_data : sorted_data) {
    ret.push_back(std::make_unique<SortedBufferDataAccess>(
        buffer_data->clone(), SortedBufferDataAccess::IndexSSBOData{
                                  Scan_local_i->cloneBufferDataInterface(),
                                  counter_i->cloneBufferDataInterface(),
                              }));
  }
  return ret;
}

