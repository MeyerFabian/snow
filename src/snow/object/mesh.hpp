#ifndef MESH_H
#define MESH_H
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/scene.h>        // Output data structure
#include <assimp/Importer.hpp>   // C++ importer interface
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include "particlesystem.hpp"
#include "texture.hpp"
struct Vertex {
 public:
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 texpos;

  Vertex() {}

  Vertex(const glm::vec3& pos, const glm::vec3& normal,
         const glm::vec2& texpos) {
    this->pos = pos;
    this->texpos = texpos;
    this->normal = normal;
  }
};

class Mesh {
 public:
  Mesh();
  ~Mesh();

  bool LoadMesh(const std::string& Filename);
  void Render() const;
  void initVBO();
  void InitParticlesFromMesh(shared_ptr<ParticleSystem> const pPs);
  void setScale(const float x, const float y, const float z) {
    scale.x = x;
    scale.y = y;
    scale.z = z;
  }
  void setRotation(const float x_angle, const float y_angle,
                   const float z_angle) {
    rotation.x = x_angle;
    rotation.y = y_angle;
    rotation.z = z_angle;
  }
  void setPosition(const float x, const float y, const float z) {
    position.x = x;
    position.y = y;
    position.z = z;
  }
  void setPosition(const glm::vec3& pos) { position = pos; }

  const glm::vec3& getPosition() const { return position; }
  const glm::vec3& getRotation() { return rotation; }
  const glm::vec3& getScale() { return scale; }

 private:
  bool InitFromScene(const aiScene* pScene, const std::string& Filename);
  void InitMesh(unsigned int Index, const aiMesh* paiMesh);
  bool InitMaterials(const aiScene* pScene, const std::string& Filename);

  void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  struct MeshEntry {
    MeshEntry();
    ~MeshEntry();

    void Init();
    GLuint VB;
    GLuint IB;
    unsigned int NumIndices;
    unsigned int MaterialIndex;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
  };
  std::vector<MeshEntry> m_Entries;
  std::vector<shared_ptr<Texture>> m_Textures;
  glm::vec3 scale;
  glm::vec3 rotation;
  glm::vec3 position;
};

#endif  // MESH_H

