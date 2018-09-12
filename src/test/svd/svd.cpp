#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
#define GLM_ENABLE_EXPERIMENTAL
#include <assert.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/rendering/GLFWWindow.hpp"
#include "../../snow/shader/technique.hpp"
class SVDTest : public Technique {
 public:
  void init() {
    Technique::add_shader(std::make_shared<Shader>(ShaderType::COMPUTE,
                                                   "shader/test/svd/svd.glsl"));
    Technique::upload();
    Technique::use();
  }
  void dispatch_with_barrier(size_t numTestMatrices) {
    glDispatchCompute(numTestMatrices / 1024, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
};

template <typename T>
int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}
float det_error(const Eigen::Matrix3f &A, float num) {
  float detA = A.determinant();
  return abs(detA - num);
}
bool check_sgn_mat_sigma3(const Eigen::Matrix3f &A, float sigma) {
  float detA = A.determinant();
  return sgn(sigma) == sgn(detA);
}

int main() {
  // Eigen SVD
  size_t numTestMatrices = 1'024;
  std::vector<Eigen::MatrixXf> testMatrices;
  testMatrices.reserve(numTestMatrices);
  std::vector<Eigen::VectorXf> singularValues;
  singularValues.reserve(numTestMatrices);
  for (int i = 0; i < numTestMatrices; i++) {
    Eigen::MatrixXf E = Eigen::Matrix3f::Random();
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(
        E, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::VectorXf S = svd.singularValues();
    // std::cout << E << std::endl;
    testMatrices.push_back(E);
    singularValues.push_back(S);
  }
  GLFWWindow();
  struct BufferData {
    Eigen::Matrix4f A;
    Eigen::Matrix4f U;
    Eigen::Matrix4f S;
    Eigen::Matrix4f V;
    Eigen::Matrix4f R;
    Eigen::Matrix4f P;
  };

  std::vector<BufferData> data;
  std::transform(std::cbegin(testMatrices), std::cend(testMatrices),
                 std::back_inserter(data), [](Eigen::MatrixXf E) {
                   E.conservativeResizeLike(Eigen::MatrixXf::Zero(4, 4));
                   return BufferData({E});
                 });

  Buffer<BufferData> buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW);
  buffer.transfer_to_gpu(data);

  buffer.gl_bind_base(1);

  auto shaderprogram = SVDTest();
  shaderprogram.init();
  shaderprogram.dispatch_with_barrier(numTestMatrices);

  auto m(buffer.transfer_to_cpu(std::size(data)));

  std::cout << "Example Random Matrices:" << std::endl;
  std::cerr << testMatrices[0] << std::endl;

  std::cout << "After composition by GLSL:" << std::endl;
  std::cerr << m[0].A << std::endl;

  double sing_val_error = 0.0;
  double comp_error = 0.0;
  double det_U_error = 0.0;
  double det_V_error = 0.0;
  unsigned int total_sign_errors_sigma3 = 0;
  for (size_t i = 0; i < numTestMatrices; i++) {
    Eigen::Matrix3f eg = testMatrices[i];
    Eigen::Vector3f esv = singularValues[i];
    // Convert back to Eigen Matrices/Vectors
    const BufferData &gl = m[i];

    Eigen::Matrix3f gl_S = gl.S.topLeftCorner<3, 3>();
    Eigen::Matrix3f gl_A = gl.A.topLeftCorner<3, 3>();

    Eigen::Matrix3f gl_U = gl.U.topLeftCorner<3, 3>();
    Eigen::Matrix3f gl_V = gl.V.topLeftCorner<3, 3>();
    Eigen::Vector3f gl_sigma = gl_S.diagonal();

    // last singular values can be by convention negative in computer graphics
    // then det(U)=det(V)=1, sgn(det(A)) = sgn(sigma(3))
    det_U_error += det_error(gl_U, 1.0f);
    det_V_error += det_error(gl_V, 1.0f);
    total_sign_errors_sigma3 += !check_sgn_mat_sigma3(gl_A, gl_sigma(2));

    gl_sigma(2) = abs(gl_sigma(2));

    // Compare with initial Matrix and Jacobi SVD singular values
    comp_error += (eg - gl_A).norm();
    sing_val_error += (esv - gl_sigma).norm();
  }
  double avg_comp_error = comp_error / numTestMatrices;
  double avg_sing_val_error = sing_val_error / numTestMatrices;
  double avg_det_u_error = det_U_error / numTestMatrices;
  double avg_det_v_error = det_V_error / numTestMatrices;
  std::cout << "Avg error of A after SVD + Recomposition :" << avg_comp_error
            << std::endl;
  std::cout << "Avg norm-2 error: Compare Eigen::JacobiSVD.singularValues() "
               "<-> GLSL-SingularValues :"
            << avg_sing_val_error << std::endl;
  std::cout << "Avg norm-1 error, det(U): " << avg_det_u_error << std::endl;
  std::cout << "Avg norm-1 error, det(V): " << avg_det_v_error << std::endl;
  std::cout << "Sign errors of Sigma3 with CG-Graphics convention: "
            << total_sign_errors_sigma3 << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  return 0;
}

