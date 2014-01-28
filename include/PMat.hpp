#ifndef PMAT_HPP
#define PMAT_HPP

#include <glm/glm.hpp>

class PMat {
protected:
    glm::vec3 m_position;
    glm::vec3 m_force;
    glm::vec3 m_velocity;
    float m_mass;

public:
    PMat():m_mass(1.f){

    }

    PMat(glm::vec3 position, glm::vec3 velocity):m_position(position), m_velocity(velocity), m_mass(1.f){

    }

    PMat(glm::vec3 position, glm::vec3 velocity, float mass):m_position(position), m_velocity(velocity), m_mass(mass){

    }

    inline void move(glm::vec3 deltapos){
        m_pos += deltapos;
    }

    inline void addFrc(glm::vec3 force){
        m_force += force;
    }

    virtual void update() = 0;

    virtual ~PMat(){}
};

#endif // PMAT_HPP
