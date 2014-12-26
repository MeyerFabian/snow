#include "math3d.h"
#include <iostream>
using namespace std;

/*
 *
 * Vector3f
 *
 * */
void Vector3f::normalize(){
    float length = sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
    this->x =this->x/length;
    this->y =this->y/length;
    this->z =this->z/length;
}
Vector3f Vector3f::cross(Vector3f& operand){
    Vector3f result;
    result.x = this->y * operand.z- this->z * operand.y;
    result.y = this->z * operand.x- this->x * operand.z;
    result.z = this->x * operand.y - this->y * operand.x;
    return result;
}
void Vector3f::print() const{
    std::cout<<"Vector3f=   " << this->x << " "<< this->y << " "<< this->z << std::endl;
}


void Vector4f::print() const{
    std::cout<<"Vector4f=   " << this->x << " "<< this->y << " "<< this->z << " " <<this->w << std::endl;
}
/*
 *
 * Matrix3f
 *
 * */

void Matrix3f::print() const {
    std::cout<<"Matrix3f=   " << this->m[0][0] << " "<< this->m[0][1] << " "<< this->m[0][2] << std::endl;
    std::cout<<"            " << this->m[1][0] << " "<< this->m[1][1] << " "<< this->m[1][2] << std::endl;
    std::cout<<"            " << this->m[2][0] << " "<< this->m[2][1] << " "<< this->m[2][2] << std::endl;
}

/*
 *
 * Matrix4f
 *
 * */
void Matrix4d::print() const {
    std::cout<<"Matrix4d=   " << this->m[0][0] << " "<< this->m[0][1] << " "<< this->m[0][2] << " "<< this->m[0][3] << std::endl;
    std::cout<<"            " << this->m[1][0] << " "<< this->m[1][1] << " "<< this->m[1][2] << " "<< this->m[1][3] << std::endl;
    std::cout<<"            " << this->m[2][0] << " "<< this->m[2][1] << " "<< this->m[2][2] << " "<< this->m[2][3] << std::endl;
    std::cout<<"            " << this->m[3][0] << " "<< this->m[3][1] << " "<< this->m[3][2] << " "<< this->m[3][3] << std::endl;
}
void Matrix4f::print() const {
    std::cout<<"Matrix4f=   " << this->m[0][0] << " "<< this->m[0][1] << " "<< this->m[0][2] << " "<< this->m[0][3] << std::endl;
    std::cout<<"            " << this->m[1][0] << " "<< this->m[1][1] << " "<< this->m[1][2] << " "<< this->m[1][3] << std::endl;
    std::cout<<"            " << this->m[2][0] << " "<< this->m[2][1] << " "<< this->m[2][2] << " "<< this->m[2][3] << std::endl;
    std::cout<<"            " << this->m[3][0] << " "<< this->m[3][1] << " "<< this->m[3][2] << " "<< this->m[3][3] << std::endl;
}

void Matrix4f::LoadScale(Vector3f& scale){
    this->m[0][0]= scale.x;  this->m[0][1]= 0.0f;  this->m[0][2]= 0.0f;  this->m[0][3]= 0.0f;
    this->m[1][0]= 0.0f;  this->m[1][1]= scale.y;  this->m[1][2]= 0.0f;  this->m[1][3]= 0.0f;
    this->m[2][0]= 0.0f;  this->m[2][1]= 0.0f;  this->m[2][2]= scale.z;  this->m[2][3]= 0.0f;
    this->m[3][0]= 0.0f;  this->m[3][1]= 0.0f;  this->m[3][2]= 0.0f;  this->m[3][3]= 1.0f;
}

void Matrix4f::LoadRotation(Vector3f& rotation){
    this->m[0][0]= 1.0f;  this->m[0][1]= 0.0f;              this->m[0][2]= 0.0f;               this->m[0][3]= 0.0f;
    this->m[1][0]= 0.0f;  this->m[1][1]= cosf(rotation.x);  this->m[1][2]= -sinf(rotation.x);  this->m[1][3]= 0.0f;
    this->m[2][0]= 0.0f;  this->m[2][1]= sinf(rotation.x);  this->m[2][2]= cosf(rotation.x);   this->m[2][3]= 0.0f;
    this->m[3][0]= 0.0f;  this->m[3][1]= 0.0f;              this->m[3][2]= 0.0f;               this->m[3][3]= 1.0f;

    Matrix4f r1;
    r1.m[0][0]=cosf(rotation.y);    r1.m[0][1]= 0.0f;  r1.m[0][2]= sinf(rotation.y); r1.m[0][3]= 0.0f;
    r1.m[1][0]=0.0f;                r1.m[1][1]= 1.0f;  r1.m[1][2]= 0.0f;             r1.m[1][3]= 0.0f;
    r1.m[2][0]=-sinf(rotation.y);   r1.m[2][1]= 0.0f;  r1.m[2][2]= cosf(rotation.y); r1.m[2][3]= 0.0f;
    r1.m[3][0]=0.0f;                r1.m[3][1]= 0.0f;  r1.m[3][2]= 0.0f;             r1.m[3][3]= 1.0f;

    Matrix4f r2;

    r2.m[0][0]= cosf(rotation.z);  r2.m[0][1]= -sinf(rotation.z);   r2.m[0][2]= 0.0f;  r2.m[0][3]= 0.0f;
    r2.m[1][0]= sinf(rotation.z);  r2.m[1][1]= cosf(rotation.z);    r2.m[1][2]= 0.0f;  r2.m[1][3]= 0.0f;
    r2.m[2][0]= 0.0f;              r2.m[2][1]= 0.0f;                r2.m[2][2]= 1.0f;  r2.m[2][3]= 0.0f;
    r2.m[3][0]= 0.0f;              r2.m[3][1]= 0.0f;                r2.m[3][2]= 0.0f;  r2.m[3][3]= 1.0f;

   *this=r2*r1*(*this);
}

void Matrix4f::LoadPosition(Vector3f& position){

    this->m[0][0]= 1.0f;  this->m[0][1]= 0.0f;  this->m[0][2]= 0.0f;  this->m[0][3]= position.x;
    this->m[1][0]= 0.0f;  this->m[1][1]= 1.0f;  this->m[1][2]= 0.0f;  this->m[1][3]= position.y;
    this->m[2][0]= 0.0f;  this->m[2][1]= 0.0f;  this->m[2][2]= 1.0f;  this->m[2][3]= position.z;
    this->m[3][0]= 0.0f;  this->m[3][1]= 0.0f;  this->m[3][2]= 0.0f;  this->m[3][3]= 1.0f;
}
void Matrix4f::LoadPerspMatrix(persp_info& m_persp){
    const float zNear = m_persp.zNear;
    const float zFar = m_persp.zFar;
    const float ar = m_persp.width /m_persp.height;
    const float tangens = tanf(toRadian(m_persp.fov/2.0f));
    const float zdif = zNear-zFar;

    this->m[0][0]= 1.0f/(tangens*ar);  this->m[0][1]= 0.0f;  this->m[0][2]= 0.0f;  this->m[0][3]= 0.0f;
    this->m[1][0]= 0.0f;  this->m[1][1]= 1.0f/(tangens);  this->m[1][2]= 0.0f;  this->m[1][3]= 0.0f;
    this->m[2][0]= 0.0f;  this->m[2][1]= 0.0f;  this->m[2][2]= ((-zNear-zFar)/zdif);  this->m[2][3]= 2.0f*zFar*zNear/(zdif);
    this->m[3][0]= 0.0f;  this->m[3][1]= 0.0f;  this->m[3][2]= 1.0f;  this->m[3][3]= 0.0f;
}


void Matrix4f::LoadCameraPosMatrix(Vector3f &pos){

    this->m[0][0]= 1.0f;  this->m[0][1]= 0.0f;  this->m[0][2]= 0.0f;  this->m[0][3]= -pos.x;
    this->m[1][0]= 0.0f;  this->m[1][1]= 1.0f;  this->m[1][2]= 0.0f;  this->m[1][3]= -pos.y;
    this->m[2][0]= 0.0f;  this->m[2][1]= 0.0f;  this->m[2][2]= 1.0f;  this->m[2][3]= -pos.z;
    this->m[3][0]= 0.0f;  this->m[3][1]= 0.0f;  this->m[3][2]= 0.0f;  this->m[3][3]= 1.0f;
}

void Matrix4f::LoadCameraMatrix(Vector3f& xAxis, Vector3f& yAxis, Vector3f& zAxis){
    this->m[0][0]= xAxis.x;  this->m[0][1]= xAxis.y;    this->m[0][2]= xAxis.z; this->m[0][3]= 0.0f;
    this->m[1][0]= yAxis.x;  this->m[1][1]= yAxis.y;    this->m[1][2]= yAxis.z; this->m[1][3]= 0.0f;
    this->m[2][0]= zAxis.x;  this->m[2][1]= zAxis.y;    this->m[2][2]= zAxis.z; this->m[2][3]= 0.0f;
    this->m[3][0]= 0.0f;     this->m[3][1]= 0.0f;       this->m[3][2]= 0.0f;    this->m[3][3]= 1.0f;
}
