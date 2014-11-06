#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H

class physicEngine{
    public :
    physicEngine() = default;
    virtual bool init() = 0;
    virtual void update(double dt) = 0;
};

#endif // PHYSICENGINE_H
