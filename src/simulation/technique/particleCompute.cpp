#include "particleCompute.h"
bool ParticleCompute::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    return true;
}
