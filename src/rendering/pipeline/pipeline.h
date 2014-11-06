#ifndef PIPELINE_H
#define PIPELINE_H


#include "../../math3d.h"
#include "camera.h"

class pipeline
{
public:
    

    const Matrix4f* getMVP();
    const Matrix4f* getModelMatrix();
    void setPerspective(float fov, float width, float height, float zNear, float zFar){
        m_persp.fov= fov;
        m_persp.width=width;
        m_persp.height=height;
        m_persp.zNear=zNear;
        m_persp.zFar=zFar;
    }

    void setScale(float x, float y, float z){
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }
    void setRotation(float x_angle, float y_angle, float z_angle){
        rotation.x = x_angle;
        rotation.y = y_angle;
        rotation.z = z_angle;
    }
    void setPosition(float x, float y, float z){
        position.x = x;
        position.y = y;
        position.z = z;
    }
    Vector3f getCameraPos();
    void setCamera(float pos_x, float pos_y, float pos_z, float lookAt_x,float lookAt_y,float lookAt_z, float up_x,float up_y,float up_z);
    void update(int key, float stepsize);

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
