#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include <GL/glew.h>
#include <Vertex.hpp>
#include <vector>
#include <string>

class aiScene;
class aiMesh;

class MeshRenderer {
private:
    struct Mesh {
        Mesh():m_numIndices(0){

        }

        ~Mesh();

        void init(const std::vector<Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        GLuint m_VB;
        GLuint m_IB;
        GLuint m_VAO;
        unsigned int m_numIndices;
    };

    std::vector<Mesh> m_Entries;

    void initFromScene(const aiScene* pScene);
    void initMesh(unsigned int Index, const aiMesh* paiMesh);

public:
    MeshRenderer(){}
    void load(const std::string& filepath);

    void render() const;
};

#endif // MESHRENDERER_HPP
