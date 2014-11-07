#include "mesh.h"
#include <assert.h>
#include <iostream>
#define INVALID_OGL_VALUE 0xffffffff
Mesh::Mesh()
{
}
Mesh::~Mesh(){
    Clear();
}


Mesh::MeshEntry::MeshEntry(){

    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    NumIndices = 0;
    MaterialIndex = INVALID_MATERIAL;

}
Mesh::MeshEntry::~MeshEntry(){
    if(VB != INVALID_OGL_VALUE){
        glDeleteBuffers(1,&VB);
    }
    if(IB != INVALID_OGL_VALUE){
        glDeleteBuffers(1,&IB);
    }
}

bool Mesh::LoadMesh(const std::string& Filename){

    Clear();
    bool Ret = false;
    
   Assimp::Importer Importer;
    
    const aiScene* pScene = Importer.ReadFile(Filename,aiProcess_Triangulate|aiProcess_GenNormals|aiProcess_FlipUVs);

    if(pScene){
       Ret = InitFromScene(pScene,Filename);

    }
    else{
        printf("Error parsing '%s': '%s'\n",Filename.c_str(),Importer.GetErrorString());
    }

    return Ret;
}

void Mesh::Render(){
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    for(unsigned int i=0; i < m_Entries.size(); i++){
        glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)24);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);
        const unsigned int MaterialIndex =m_Entries[i].MaterialIndex;
        //std::cout << m_Textures[MaterialIndex]->filename << std::endl;
        if(MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]){
            m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
        }
        glDrawElements(GL_TRIANGLES,m_Entries[i].NumIndices,GL_UNSIGNED_INT,0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename){
   
        m_Entries.resize(pScene->mNumMeshes);
        m_Textures.resize(pScene->mNumMaterials);
        for(unsigned int i = 0; i < m_Entries.size();i++){

            const aiMesh* paiMesh = pScene->mMeshes[i];
            InitMesh(i, paiMesh);

        }

        return InitMaterials(pScene,Filename);
  
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh){

    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

    const aiVector3D Zero3D(0.0f,0.0f,0.0f);

    for(unsigned int i = 0 ; i< paiMesh->mNumVertices;i++){

        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]): &Zero3D;

        Vector3f pos =Vector3f(pPos->x,pPos->y,pPos->z);
        Vector3f normal =Vector3f(pNormal->x, pNormal->y,pNormal->z);
        Vector2f texture =Vector2f(pTexCoord->x,pTexCoord->y);
        m_Entries[Index].Vertices.push_back(Vertex(pos,normal,texture));
    }

    for(unsigned int i = 0; i < paiMesh->mNumFaces; i++){
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        m_Entries[Index].Indices.push_back(Face.mIndices[0]);
        m_Entries[Index].Indices.push_back(Face.mIndices[1]);
        m_Entries[Index].Indices.push_back(Face.mIndices[2]);
    }

    //m_Entries[Index].Init(Vertices,Indices);

    
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename){

    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;
    if(SlashIndex == std::string::npos){
        Dir = ".";
    }
    else if(SlashIndex == 0){
        Dir = "/";
    }
    else{
        Dir = Filename.substr(0,SlashIndex);
    }

    bool Ret = true;

    for(unsigned int i= 0; i< pScene->mNumMaterials ; i++){

        const aiMaterial* pMaterial = pScene->mMaterials[i];


        if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE)>0){
            aiString Path;
            if(pMaterial->GetTexture(aiTextureType_DIFFUSE,0,&Path, NULL,NULL,NULL,NULL,NULL)== AI_SUCCESS){
            //std::cout <<Path.data<<std::endl;

                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = make_shared<Texture>(FullPath);

            }
        }
    }

    return Ret;

}

void Mesh::Clear(){


}
void Mesh::initVBO(){
    for(auto& meshEntry : m_Entries){
        meshEntry.Init();
    }

    for (int i = 0; i<m_Textures.size();i++){
        if(m_Textures[i]== nullptr || !m_Textures[i]->Load(GL_TEXTURE_2D)){
            std::cout<< "Error loading texture"<<endl;

        }
        else{
            std::cout << "Loaded Texture" << m_Textures[i]->filename<<endl;
        }

    }
}

void Mesh::MeshEntry::Init(){

    NumIndices = Indices.size();
    glGenBuffers(1,&VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1,&IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * NumIndices,&Indices[0], GL_STATIC_DRAW);

}
