#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

class renderingEngine{
    public :
    renderingEngine() = default;
    virtual bool init() = 0;
    virtual void render() = 0;
    virtual bool shouldClose() = 0;
    virtual void stop() = 0;
};

#endif // RENDERINGENGINE_H
