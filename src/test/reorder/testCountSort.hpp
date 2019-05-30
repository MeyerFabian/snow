#ifndef TEST_COUNT_SORT_HPP_MZVEWHYI
#define TEST_COUNT_SORT_HPP_MZVEWHYI

#include "../OutputBufferData.hpp"
class TestCountSort {
 public:
  struct TestCountSortData {
    GLuint numGridPoints;
    BufferLayout layout;
  };

  TestCountSort(TestCountSortData&& tc, OutputBufferData bd) {
#ifdef SORTED
    CountingSortPipeline::CountingSortData cnt_srt_data{
        tc.layout,
        tc.numGridPoints,
    };

    IOBufferData io_cnt_srt;
    // OUTPUT
    io_cnt_srt.out_buffer.push_back(
        std::make_unique<BufferData>(bd.Particle_pos_unsorted));
    io_cnt_srt.out_buffer.push_back(
        std::make_unique<BufferData>(bd.Particle_2_unsorted));
#endif

#if defined(FULL_SORTED)
    cnt_srt_pipeline.initFullSort(std::move(cnt_srt_data),
                                  std::move(io_cnt_srt));
#elif defined(INDEX_SORTED)
#if defined(INDEX_WRITE_SORTED)
    cnt_srt_pipeline.initIndexWriteSort(std::move(cnt_srt_data),
                                        std::move(io_cnt_srt));
#else

    cnt_srt_pipeline.initIndexReadSort(std::move(cnt_srt_data),
                                       std::move(io_cnt_srt));
#endif
#endif
  }

  std::vector<std::unique_ptr<SortedBufferData> > getSortedBufferData() {
    return cnt_srt_pipeline.getSortedBufferData();
  }

  std::vector<std::unique_ptr<SortedBufferDataAccess> >
  getSortedBufferDataAccess() {
    return cnt_srt_pipeline.getSortedBufferDataAccess();
  }

  std::unique_ptr<BufferDataInterface> getGridCounter() {
    return cnt_srt_pipeline.getGridCounter();
  }
  void run(GLuint numGridPoints, GLuint numParticles) {
#ifdef FULL_SORTED
    cnt_srt_pipeline.run({numParticles, numGridPoints});
#endif
  }

 private:
  CountingSortPipeline cnt_srt_pipeline;
};

#endif /* end of include guard: COUNT_SORT_HPP_MZVEWHYI */

