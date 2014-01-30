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

    PMat(const glm::vec3& position):
        m_position(position),
        m_mass(1.f){

    }

    PMat(const glm::vec3& position, const float mass):
        m_position(position),
        m_mass(mass){

    }

    inline void addFrc(glm::vec3 force){
        m_force += force;
    }

    const glm::vec3& getPosition() const {
        return m_position;
    }

    const float& getMass() const {
        return m_mass;
    }

    virtual void update() = 0;

    virtual ~PMat(){}
};

#endif // PMAT_HPP
