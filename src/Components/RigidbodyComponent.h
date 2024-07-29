#ifndef RigidbodyComponent_H
#define RigidbodyComponent_H
#include <glm/glm.hpp>

struct RigidbodyComponent 
{
    glm::vec2 velocity; 
    //contructor
    RigidbodyComponent(glm::vec2 vel = glm::vec2(0.0f,0.0f))
    {
        this->velocity = vel;
    };
};
#endif