#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H
#include <memory>
#include <vector>
#include "object/mesh.h"
class renderingEngine{
    public :
    renderingEngine() = default;
    virtual bool init( std::shared_ptr<std::vector< Mesh >> meshes ) = 0;
    virtual void render() = 0;
    virtual bool shouldClose() = 0;
    virtual void stop() = 0;
};

#endif // RENDERINGENGINE_H
