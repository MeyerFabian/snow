#ifndef GLM_INC_GUARD
#define GLM_INC_GUARD
#include <glm/gtc/random.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#endif

#ifndef NUMVECTORS
#define NUMVECTORS 1024*1024-1
#endif

#ifndef INIT_DATA
#define INIT_DATA PREC_VEC_TYPE(1.0f,0.0f,0.0f, 0.0f)
#endif /* ifndef INIT_DATA */


#include "mapReduce.cpp"