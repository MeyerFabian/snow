#include "countingSortPipeline.hpp"

void CountingSortPipeline::init(CountingSortData&& cnt_srt_data,
                                IOBufferData&& io_data) {
  GLuint gridSize = cnt_srt_data.gGridDim.x * cnt_srt_data.gGridDim.y +
                    cnt_srt_data.gGridDim.z;
  GLuint to_sort_size = io_data.in_buffer[0]->getSize();

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
  gridOffsets_buffer->gl_bind_base(PARTICLE_INDICES_BINDING);

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
  auto counter_i = BufferData("counters", "Counter_i",
                              binning_buffer->get_buffer_info(), gridSize);

  // gridOffset to_sort
  auto gridOffset_i = BufferData(
      "particle_indices", "GridOffset_i", gridOffsets_buffer->get_buffer_info(),
      to_sort_size, 1, 0, "ParticleIndices_VAR_SIZE");

  // scan       grid
  auto Scan_local_i =
      BufferData("scans", "Scan_local_i", scan_buffer->get_buffer_info(),
                 gridSize, 1, 0, "Scan_VAR_SIZE");
  auto Scan_block_i =
      BufferData("scans", "Scan_block_i", scan_buffer->get_buffer_info(),
                 gridSize, 1, 0, "Scan_VAR_SIZE");

  /**********************************************************************
   *                         Technique/Shaders                          *
   **********************************************************************/

  // Reset
  MapTechnique::MapData map_data{
      "shader/compute/mapreduce/map.glsl",
      // unary_op
      "0",
  };

  IOBufferData io_map;
  // INPUT
  io_map.in_buffer.push_back(std::make_unique<BufferData>(counter_i));
  // OUTPUT
  io_map.out_buffer.push_back(std::make_unique<BufferData>(counter_i));

  resetCounter.init(std::move(map_data), std::move(io_map));

  BinningTechnique::BinningData binning_data{
      "shader/compute/preprocess/bin.glsl",
      cnt_srt_data.gGridPos,
      cnt_srt_data.gGridDim,
      cnt_srt_data.gridSpacing,

  };
  /*
  IOBufferData io_bin;
  // INPUT
  io_bin.in_buffer.push_back(sorting_data[0].cloneBufferDataInterface());

  // OUTPUT
  io_bin.out_buffer.push_back(std::make_unique<BufferData>(counter_i));

  // OUTPUT2
  io_bin.out_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));

  binCount.init(std::move(binning_data), std::move(io_bin));
  */
}
void CountingSortPipeline::run(CountingSortDispatch&& dispatch_data) {
  // reset
  //
  /*
  BenchmarkerGPU::getInstance().time(
      "resetCounter", [this, numGridPoints = dispatch_data.numGridPoints]() {
        resetCounter.dispatch_with_barrier({numGridPoints, true, 2});
      });

  // bin
  BenchmarkerGPU::getInstance().time(
      "Counter", [this, numParticles = dispatch_data.numParticles]() {
        binCount.dispatch_with_barrier(numParticles);
      });
        // scan
        scanPipeline.run(numGridPoints);
        // reorder
        BenchmarkerGPU::getInstance().time(
            "Reorder Particles", [&reordering, numParticles]() {
              reordering.dispatch_with_barrier({numParticles});
            });
   */
}
/*
void CountingSortPipeline::initSortedBufferData(

) {
for (auto it = io_data.out_buffer.begin(); it != io_data.out_buffer.end();
 it++) {
sorting_data.push_back(SortedBufferData(
  SortingMethod::Full, io_data.in_buffer[0]->cloneBufferDataInterface()));
}
}
*/

void CountingSortPipeline::initFullSort(CountingSortData&& cnt_srt_data,
                                        IOBufferData&& io_data) {
  /*
initSortedBufferData(std::move(io_data));
*/
}

void CountingSortPipeline::initIndexSort(CountingSortData&& cnt_srt_data,
                                         IOBufferData&& io_data) {
  /*
GLuint to_sort_db_size = io_data.in_buffer[0]->getSize() * 2;
index_buffer = std::make_unique<Buffer<GLuint> >(
BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
"shader/buffers/******needs to be created ******.include.glsl");
index_buffer.resize(to_sort_db_size);
index_buffer.gl_bind_base(PARTICLE_INDICES_BINDING); */
  /*
    auto unsortedIndex_i =
        BufferData("particle_indices", "UnsortedIndex_i",
                   particle_indices_buffer.get_buffer_info(), data.numParticles,
  */
}

