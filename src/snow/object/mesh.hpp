#ifndef MESH_H
#define MESH_H
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/scene.h>        // Output data structure
#include <assimp/Importer.hpp>   // C++ importer interface
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <memory>
#include "math3d.hpp"
#include "particlesystem.hpp"
#include "texture.hpp"
struct Vertex {
 public:
  Vector3f pos;
  Vector3f normal;
  Vector2f texpos;

  Vertex() {}

  Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texpos) {
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
  void Render();
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
  void setPosition(const Vector3f& pos) { position = pos; }

  const Vector3f& getPosition() const { return position; }
  const Vector3f& getRotation() { return rotation; }
  const Vector3f& getScale() { return scale; }

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
  Vector3f scale;
  Vector3f rotation;
  Vector3f position;
};

#endif  // MESH_H
