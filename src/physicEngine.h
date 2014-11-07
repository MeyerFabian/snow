#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H
#include <memory>
#include <vector>
#include "object/mesh.h"

class physicEngine{
    public :
    physicEngine() = default;
    virtual bool init() = 0;
    virtual void update( std::shared_ptr<std::vector< Mesh >> meshes,double dt) = 0;
};

#endif // PHYSICENGINE_H
