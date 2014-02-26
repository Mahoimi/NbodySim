#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

#endif // VERTEX_HPP
