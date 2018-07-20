#ifndef MATH3D_H
#define MATH3D_H

#include <cmath>

#define M_PI 3.14159265358979323846
#define toDegree(x) (float)(x * 180.0f / M_PI)
#define toRadian(x) (float)(x * M_PI / 180.0f)

struct Vector2f {
  float x;
  float y;
  Vector2f() {
    this->x = 0;
    this->y = 0;
  }
  Vector2f(const Vector2f& vec) {
    this->x = vec.x;
    this->y = vec.y;
  }

  Vector2f(float x, float y) {
    this->x = x;
    this->y = y;
  }
};

struct Vector3f {
  float x;
  float y;
  float z;
  Vector3f() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  Vector3f(const Vector3f& vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
  }

  Vector3f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  void normalize();
  void print() const;
  Vector3f cross(Vector3f& operand);

  inline Vector3f operator+(const Vector3f operand) {
    Vector3f result;
    result.x = this->x + operand.x;
    result.y = this->y + operand.y;
    result.z = this->z + operand.z;
    return result;
  }
  inline Vector3f operator*(const float operand) {
    Vector3f result;
    result.x = this->x * operand;
    result.y = this->y * operand;
    result.z = this->z * operand;
    return result;
  }

  inline Vector3f operator-(const Vector3f operand) {
    Vector3f result;
    result.x = this->x - operand.x;
    result.y = this->y - operand.y;
    result.z = this->z - operand.z;
    return result;
  }
};
struct Vector4f {
  float x;
  float y;
  float z;
  float w;
  Vector4f(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  Vector4f(const Vector3f& vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = 0;
  }
  Vector4f(const Vector3f& vec, const float w) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = w;
  }
  void print() const;
  Vector3f xyz();
};
struct Vector3i {
  int x;
  int y;
  int z;
  Vector3i(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  void print() const;
};
struct Vector4i {
  int x;
  int y;
  int z;
  int w;
  Vector4i(int x, int y, int z, int w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  Vector4i(const Vector3i& vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = 0;
  }
  void print() const;
};

struct persp_info {
  float fov;
  float width;
  float height;
  float zNear;
  float zFar;
};

struct Matrix3f {
  Matrix3f() = default;
  Matrix3f(float f) {
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 3; i++) {
        if (j == i) {
          this->m[j][i] = f;
        } else {
          this->m[j][i] = 0.0f;
        }
      }
    }
  }
  float m[3][3];

  inline void LoadIdentity() {
    this->m[0][0] = 1.0f;
    this->m[0][1] = 0.0f;
    this->m[0][2] = 0.0f;
    this->m[1][0] = 0.0f;
    this->m[1][1] = 1.0f;
    this->m[1][2] = 0.0f;
    this->m[2][0] = 0.0f;
    this->m[2][1] = 0.0f;
    this->m[2][2] = 1.0f;
  }
  inline Matrix3f operator*(const Matrix3f operand) {
    Matrix3f returnMatrix;
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < 3; i++) {
        returnMatrix.m[j][i] = this->m[j][0] * operand.m[0][i] +
                               this->m[j][1] * operand.m[1][i] +
                               this->m[j][2] * operand.m[2][i];
      }
    }
    return returnMatrix;
  }

  void print() const;
};
struct Matrix4d {
  double m[4][4];
  Matrix4d() = default;
  Matrix4d(double f) {
    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 4; i++) {
        if (j == i) {
          this->m[j][i] = f;
        } else {
          this->m[j][i] = 0.0f;
        }
      }
    }
  }
  void print() const;
};

struct Matrix4f {
  float m[4][4];
  Matrix4f() = default;
  Matrix4f(float f) {
    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 4; i++) {
        if (j == i) {
          this->m[j][i] = f;
        } else {
          this->m[j][i] = 0.0f;
        }
      }
    }
  }
  inline void LoadIdentity() {
    this->m[0][0] = 1.0f;
    this->m[0][1] = 0.0f;
    this->m[0][2] = 0.0f;
    this->m[0][3] = 0.0f;
    this->m[1][0] = 0.0f;
    this->m[1][1] = 1.0f;
    this->m[1][2] = 0.0f;
    this->m[1][3] = 0.0f;
    this->m[2][0] = 0.0f;
    this->m[2][1] = 0.0f;
    this->m[2][2] = 1.0f;
    this->m[2][3] = 0.0f;
    this->m[3][0] = 0.0f;
    this->m[3][1] = 0.0f;
    this->m[3][2] = 0.0f;
    this->m[3][3] = 1.0f;
  }
  Matrix4f(float f1, float f2, float f3, float f4, float f5, float f6, float f7,
           float f8, float f9, float f10, float f11, float f12, float f13,
           float f14, float f15, float f16) {
    this->m[0][0] = f1;
    this->m[0][1] = f2;
    this->m[0][2] = f3;
    this->m[0][3] = f4;
    this->m[1][0] = f5;
    this->m[1][1] = f6;
    this->m[1][2] = f7;
    this->m[1][3] = f8;
    this->m[2][0] = f9;
    this->m[2][1] = f10;
    this->m[2][2] = f11;
    this->m[2][3] = f12;
    this->m[3][0] = f13;
    this->m[3][1] = f14;
    this->m[3][2] = f15;
    this->m[3][3] = f16;
  }

  inline Matrix4f operator*(const Matrix4f operand) {
    Matrix4f returnMatrix;
    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 4; i++) {
        returnMatrix.m[j][i] =
            this->m[j][0] * operand.m[0][i] + this->m[j][1] * operand.m[1][i] +
            this->m[j][2] * operand.m[2][i] + this->m[j][3] * operand.m[3][i];
      }
    }
    return returnMatrix;
  }

  void LoadPerspMatrix(persp_info& mpersp);
  void LoadScale(Vector3f& scale);
  void LoadRotation(Vector3f& rotation);
  void LoadPosition(Vector3f& position);
  void LoadCameraMatrix(Vector3f& xAxis, Vector3f& yAxis, Vector3f& zAxis);
  void LoadCameraPosMatrix(Vector3f& pos);
  void print() const;
};

#endif  // MATH3D_H
