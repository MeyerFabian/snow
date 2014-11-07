#ifndef MYRENDERINGENGINE_H
#define MYRENDERINGENGINE_H
#include "../renderingEngine.h"
class myRenderingEngine : public renderingEngine
{
public:
    myRenderingEngine() = default;
    virtual bool init(std::shared_ptr<std::vector< Mesh >> meshes);
    virtual void render();
    virtual bool shouldClose();
    virtual void stop();

};

#endif // MYRENDERINGENGINE_H
