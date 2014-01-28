#ifndef GRAVITYLINK_HPP
#define GRAVITYLINK_HPP

#include <PMat.hpp>

class GravityLink {
protected:
    PMat* m_PMat1;
    PMat* m_PMat2;

public:
    GravityLink();
    GravityLink(PMat*, PMat*);
    virtual void update() = 0;
};

#endif // GRAVITYLINK_HPP
