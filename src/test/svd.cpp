#include <Eigen/Core>
#include <Eigen/SVD>
#include <iostream>
#include <memory>
#include "../snow/rendering/GLFWWindow.hpp"
#include "../snow/shader/technique.hpp"

class SVDTest : public Technique {
 public:
  void init() {
    Technique::add_shader(
        std::make_shared<Shader>(ShaderType::COMPUTE, "shader/test/svd.glsl"));
    Technique::upload();
    Technique::use();
  }
};

int main() {
  // Eigen SVD
  Eigen::Matrix3f E = Eigen::Matrix3f::Random();
  Eigen::JacobiSVD<Eigen::MatrixXf> svd(
      E, Eigen::ComputeFullU | Eigen::ComputeFullV);
  Eigen::Matrix3f U = svd.matrixU();
  Eigen::Matrix3f S = svd.singularValues().asDiagonal();
  Eigen::Matrix3f V = svd.matrixV();
  Eigen::Matrix3f V_T = V.transpose();
  std::cout << "\nEigen library: " << std::endl;
  std::cout << "Matrix A is:" << std::endl << E << std::endl;
  std::cout << "Its singular values are:" << std::endl
            << svd.singularValues() << std::endl;
  std::cout << "Its left singular vectors are the columns of the thin U matrix:"
            << std::endl
            << U << std::endl;
  std::cout << "Its right singular vectors are the columns of the thin V "
               "matrix:"
            << std::endl
            << V << std::endl;

  Eigen::Matrix3f R = U * V_T;
  std::cout << "R: is " << std::endl << R << std::endl;
  Eigen::Matrix3f P = V * S * V_T;
  std::cout << "P: is " << std::endl << P << std::endl;
  std::cout << "Result: A" << std::endl;
  Eigen::Matrix3f SVD = U * S * V_T;
  std::cout << "For SVD:" << std::endl << SVD << std::endl;
  std::cout << "difference" << (SVD - E).array().abs().sum() << "\n";
  std::cout << "For PD:" << std::endl << R * P << std::endl;
  // GLSL SVD
  std::cerr << "\nGLSL" << std::endl;
  GLFWWindow();
  struct BufferData {
    Matrix4f A;
    Matrix4f U;
    Matrix4f S;
    Matrix4f V;
    Matrix4f R;
    Matrix4f P;
    void print() {
      std::cerr << "A:" << std::endl;
      A.print();
      std::cerr << "U:" << std::endl;
      U.print();
      std::cerr << "S:" << std::endl;
      S.print();
      std::cerr << "V:" << std::endl;
      V.print();
      std::cerr << "R:" << std::endl;
      R.print();
      std::cerr << "P:" << std::endl;
      P.print();
    }
  };

  Matrix4f A =
      Matrix4f(E(0, 0), E(0, 1), E(0, 2), 0.0f, E(1, 0), E(1, 1), E(1, 2), 0.0f,
               E(2, 0), E(2, 1), E(2, 2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  A.print();
  GLuint buffer_handle;
  glGenBuffers(1, &buffer_handle);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_handle);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(BufferData) * (1), NULL,
               GL_STATIC_DRAW);

  // We are not the owner of this pointer, no delete required.
  auto ptr = (BufferData *)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(BufferData) * (1),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  ptr[0] = BufferData({A});

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer_handle);

  auto shaderprogram = SVDTest();
  shaderprogram.init();
  glDispatchCompute(1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_handle);
  std::cout << "BufferData" << std::endl;
  auto m = (BufferData *)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(BufferData), GL_MAP_READ_BIT));

  m[0].print();

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  GLFWWindow::swapBuffers();
  glDeleteBuffers(1, &buffer_handle);

  GLFWWindow::stop();
  return 0;
}

