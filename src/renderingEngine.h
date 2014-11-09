#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H
#include <memory>
#include <vector>
#include "object/mesh.h"
#include "object/particlesystem.h"
class renderingEngine{
    public :
    renderingEngine(std::shared_ptr<std::vector< Mesh >> const meshesToRender, shared_ptr<ParticleSystem> const particlesToRender):meshes(meshesToRender), particlesystem(particlesToRender){}

    virtual bool init() = 0;
    virtual void render() = 0;
    virtual bool shouldClose() = 0;
    virtual void stop() = 0;

    std::shared_ptr<std::vector< Mesh >> const  meshes;
    std::shared_ptr<ParticleSystem> const  particlesystem;
};

#endif // RENDERINGENGINE_H
