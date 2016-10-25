#ifndef PIPELINE_H
#define PIPELINE_H


#include "math3d.h"
#include "camera.h"

class pipeline
{
public:
    

    const Matrix4f* getMVP();
    const Matrix4f* getModelMatrix();
    void setPerspective(const float fov, const float width, const float height, const float zNear, const float zFar){
        m_persp.fov= fov;
        m_persp.width=width;
        m_persp.height=height;
        m_persp.zNear=zNear;
        m_persp.zFar=zFar;
    }

    void setScale(const Vector3f &scl){
        scale = scl;
    }
    void setRotation(const Vector3f &angle){
        rotation =angle;
    }
    void setPosition(const Vector3f &pos){
        position =pos;
    }
    Vector3f getCameraPos();
    void setCamera(const float pos_x, const float pos_y, const float pos_z, const float lookAt_x,const float lookAt_y,const float lookAt_z, const float up_x,const float up_y,const float up_z);
    void update(const int key, const float stepsize);
    void update(const double xpos,const double ypos);

private:
    Vector3f scale;
    Vector3f rotation;
    Vector3f position;
    Matrix4f modelMatrix;
    Matrix4f MVP;
    persp_info m_persp;
    Camera m_camera;
};

#endif // PIPELINE_H
