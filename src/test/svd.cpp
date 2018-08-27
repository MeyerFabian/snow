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
  size_t numTestMatrices = 10;
  std::vector<Eigen::Matrix3f> testMatrices;
  testMatrices.reserve(numTestMatrices);
  std::vector<Eigen::Vector3f> singularValues;
  singularValues.reserve(numTestMatrices);
  for (int i = 0; i < numTestMatrices; i++) {
    Eigen::Matrix3f E = Eigen::Matrix3f::Random();
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(
        E, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Vector3f S = svd.singularValues();
    testMatrices.push_back(E);
    singularValues.push_back(S);
  }
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
  GLuint buffer_handle;
  glGenBuffers(1, &buffer_handle);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_handle);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(BufferData) * (testMatrices.size()), NULL,
               GL_STATIC_DRAW);

  // We are not the owner of this pointer, no delete required.
  auto ptr = (BufferData *)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(BufferData) * (testMatrices.size()),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (size_t i = 0; i < testMatrices.size(); i++) {
    Eigen::Matrix3f &E = testMatrices[i];
    Matrix4f A =
        Matrix4f(E(0, 0), E(0, 1), E(0, 2), 0.0f, E(1, 0), E(1, 1), E(1, 2),
                 0.0f, E(2, 0), E(2, 1), E(2, 2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    ptr[i] = BufferData({A});
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer_handle);

  auto shaderprogram = SVDTest();
  shaderprogram.init();
  glDispatchCompute(testMatrices.size(), 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_handle);
  std::cout << "BufferData" << std::endl;
  auto m = (BufferData *)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(BufferData) * testMatrices.size(),
      GL_MAP_READ_BIT));

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  GLFWWindow::swapBuffers();
  glDeleteBuffers(1, &buffer_handle);

  GLFWWindow::stop();
  return 0;
}

