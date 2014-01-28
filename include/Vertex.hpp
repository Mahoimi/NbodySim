#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex {
public:
    glm::vec3 position, normal;
    glm::vec2 texCoords;
};

#endif // VERTEX_HPP
