#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <GL/glew.h>

class Circle {
private:
    GLuint m_VAO;
    GLuint m_VBO[2];
    unsigned int m_verticesCount;

public:
    Circle(){}
    void init(unsigned int numPoints);
    void render() const;
    ~Circle();
};

#endif // CIRCLE_HPP
