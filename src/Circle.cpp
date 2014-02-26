#include <Circle.hpp>
#include <Vertex.hpp>
#include <vector>
#include <cassert>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void Circle::init(unsigned int numPoints){
    assert(numPoints > 8);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> index;

    // Define the center of the circle
    Vertex center;
    center.position = glm::vec3(0);
    center.normal = glm::vec3(0,0,1);
    vertices.push_back(center);

    for (unsigned int i = 0; i < numPoints+1; ++i){
        float angle = i * 2 * M_PI / numPoints;
        Vertex vertex;
        vertex.position = glm::vec3(cos(angle), sin(angle), 0);
        vertex.normal = glm::vec3(0,0,1);
        vertices.push_back(vertex);
    }

    // Index buffer data
    for (unsigned int i = 1; i < numPoints+1; ++i){
        index.push_back(0);
        index.push_back(i);
        index.push_back(i+1);
    }

    m_verticesCount = index.size();

    // Generate 2 vertex buffer objects and 1 vertex array object
    glGenBuffers(2, m_VBO);
    glGenVertexArrays(1, &m_VAO);

    // Lock the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[0]);

    // Write the data inside the IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*index.size(), &index[0], GL_STATIC_DRAW);

    // Unlock the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Lock the VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);

    // Write the data inside the VBO
    // Warning ! m_vertices.data() != &m_vertices[0]
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Unlock the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Lock the VAO
    glBindVertexArray(m_VAO);

        // Lock the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));

        // Unlock the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unlock the VAO
    glBindVertexArray(0);
}

void Circle::render() const {
    glBindVertexArray(m_VAO);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[0]);

           glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, 0);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

Circle::~Circle(){
    glDeleteBuffers(2, m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}
