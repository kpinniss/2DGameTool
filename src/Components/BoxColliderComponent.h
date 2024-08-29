#ifndef BOXCOLLIDER_COMPONENT_H
#define BOXCOLLIDER_COMPONENT_H
#include "CollisionComponent.h"
#include <glm/glm.hpp>

struct BoxColliderComponent: public CollisionComponent
{
    int height,width;
    glm::vec2 offset;

    BoxColliderComponent(int height = 0, int width = 0, glm::vec2 offset = glm::vec2(0))
    {
        this->height = height;
        this->width = width;
        this->offset = offset;
    }
};

#endif