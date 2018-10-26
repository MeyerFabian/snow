#ifndef COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#define COUNTINGSORTPIPELINE_HPP_W5WCXFCO
#include "../SortedBufferData.hpp"
#include "../map/mapTechnique.hpp"
class CountingSortPipeline {
 public:
  void init(CountingSortData&& cnt_srt_data&& IOBufferData&& io_data,
            SortingMethod);

 private:
  void initIndexSort(CountingSortData&& cnt_srt_data&&, IOBufferData&& io_data);
  void initFullSort(CountingSortData&& cnt_srt_data&& IOBufferData&& io_data);

  SortedBufferData getActiveSortedBufferData();

  std::vector<GLuint> gridOffsets;
  SortedBufferData activeBuffer;
  MapTechnique resetCounter;
};
#endif /* end of include guard: COUNTINGSORTPIPELINE_HPP_W5WCXFCO */

