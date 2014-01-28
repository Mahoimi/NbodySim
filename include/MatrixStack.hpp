#ifndef MATRIXSTACK_HPP
#define MATRIXSTACK_HPP

#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MatrixStack {
private:
        std::stack<glm::mat4> m_stack;

public:
    MatrixStack(){
        m_stack.push(glm::mat4(1.f));
    }

        inline const glm::mat4& top() const{
                return m_stack.top();
        }

        inline void set(const glm::mat4& matrix){
        m_stack.top() = matrix;
    }

    inline void mult(const glm::mat4& matrix){
        m_stack.push(m_stack.top()*matrix);
    }

    inline void pop(){
        m_stack.pop();
    }

    inline void push(){
        m_stack.push(m_stack.top());
    }

    inline void scale(const glm::vec3& s){
        set(glm::scale(m_stack.top(), s));
    }

    inline void translate(const glm::vec3& t){
        set(glm::translate(m_stack.top(), t));
    }

    inline void rotate(float degrees, const glm::vec3& r){
        set(glm::rotate(m_stack.top(), degrees, r));
    }
};

#endif // MATRIXSTACK_H
