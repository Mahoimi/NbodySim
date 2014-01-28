#include "MeshRenderer.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>

void MeshRenderer::Mesh::init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices){
    //Generate VB, IB and VAO
    glGenBuffers(1, &m_VB);
    glGenBuffers(1, &m_IB);
    glGenVertexArrays(1, &m_VAO);

    m_numIndices = Indices.size();

    // Lock the VAO
    glBindVertexArray(m_VAO);

    // Lock the VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, &Indices[0], GL_STATIC_DRAW);

    // Unlock the VAO & current buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

MeshRenderer::Mesh::~Mesh(){
    // Free memory
    if (glIsBuffer(m_VB) == GL_TRUE)
    {
        glDeleteBuffers(1, &m_VB);
    }

    if (glIsBuffer(m_IB) == GL_TRUE)
    {
        glDeleteBuffers(1, &m_IB);
    }

    if(glIsVertexArray(m_VAO) == GL_TRUE){
        glDeleteVertexArrays(1, &m_VAO);
    }
}

void MeshRenderer::load(const std::string& filepath){
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
        initFromScene(pScene);
    }
    else {
        printf("Error parsing '%s': '%s'\n", filepath.c_str(), Importer.GetErrorString());
    }

}

void MeshRenderer::initFromScene(const aiScene* pScene){
    m_Entries.resize(pScene->mNumMeshes);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }
}

void MeshRenderer::initMesh(unsigned int Index, const aiMesh* paiMesh){

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v;
        v.position=glm::vec3(pPos->x, pPos->y, pPos->z);
        v.texCoords=glm::vec2(pTexCoord->x, pTexCoord->y);
        v.normal=glm::vec3(pNormal->x, pNormal->y, pNormal->z);

        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].init(Vertices, Indices);
}

void MeshRenderer::render() const{
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        glBindVertexArray(m_Entries[i].m_VAO);
        glDrawElements(GL_TRIANGLES, m_Entries[i].m_numIndices, GL_UNSIGNED_INT, 0);
    }
}
