#ifndef MESH_H
#define MESH_H
#include <Importer.hpp>      // C++ importer interface
#include <scene.h>       // Output data structure
#include <postprocess.h> // Post processing flags
#include <vector>
#include <string>
#define GLEW_STATIC
#include "glew.h"
#include "../math3d.h"
#include "texture.h"

struct Vertex{
public:
    Vector3f pos;
    Vector3f normal;
    Vector2f texpos;

    Vertex(){}

    Vertex(const Vector3f& pos,const Vector3f& normal, const Vector2f& texpos){
        this->pos = pos;
        this->texpos = texpos;
        this->normal = normal;
    }

};

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    bool LoadMesh(const std::string& Filename);
    void Render();
private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
    struct MeshEntry{
        MeshEntry();
        ~MeshEntry();

        void Init(const std::vector<Vertex>& Vertices, const std::vector< unsigned int>& Indices);
        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };
    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
    
};

#endif // MESH_H
