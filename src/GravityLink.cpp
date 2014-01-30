#include <GravityLink.hpp>
#include <cassert>

void GravityLink::setParticles(PMat* pmat1, PMat* pmat2){
    assert(pmat1 != nullptr);
    assert(pmat2 != nullptr);
    m_PMat1 = pmat1;
    m_PMat2 = pmat2;
}

void GravityLink::update(){
    const float dist = glm::distance(m_PMat1->getPosition(), m_PMat2->getPosition());
    float attractionForce = m_PMat1->getMass() * m_PMat2->getMass() * 0.001f / (dist*dist);
    if (dist > 0.3f){
        m_PMat1->addFrc(-attractionForce * glm::normalize(m_PMat1->getPosition() - m_PMat2->getPosition()));
        m_PMat2->addFrc(attractionForce * glm::normalize(m_PMat1->getPosition() - m_PMat2->getPosition()));
    }
}
