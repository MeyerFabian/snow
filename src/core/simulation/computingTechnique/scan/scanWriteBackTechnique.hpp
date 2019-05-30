#ifndef SCANWRITEBACKTECHNIQUE_H
#define SCANWRITEBACKTECHNIQUE_H
#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/buffer.hpp"
#include "../../../shader/technique.hpp"

/*
 * Is basically a map/copy technique but spreads value:
 * e.g.
 * | count            | 3 | 6 | 8 | 7 | 2 | 5 | 5 | 1 |
 *
 * | scan local       | 0 | 3 | 9 | 17| 0 | 2 | 7 | 12|
 *                                   (+)
 * | scan block       |        0      |        24     |
 *                                   (=)
 * | scan local after | 0 | 3 | 9 | 17| 24| 26| 31| 36|
 */
class ScanWriteBackTechnique : public Technique {
 public:
  LocalSize local_size = {1024, 1, 1};
  struct ScanWriteBackData {
    std::string gl_binary_op;
    GLuint numVectors;
    GLuint scanBlockSize;
  };

  void init(ScanWriteBackData&& data, IOBufferData&& io);
  void dispatch_with_barrier(GLuint numVectors) const;
};
#endif

