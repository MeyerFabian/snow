#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H
#include <memory>
#include <vector>
#include "object/mesh.h"

class physicEngine{
    public :
    physicEngine() = default;
    virtual bool init( std::shared_ptr<std::vector< Mesh >> meshes) = 0;
    virtual void update(double dt) = 0;
};

#endif // PHYSICENGINE_H
