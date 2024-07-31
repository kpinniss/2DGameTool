#ifndef SPRIT_COMPONENT_H
#define SPRIT_COMPONENT_H
#include <glm/glm.hpp>

struct SpriteComponent 
{
    int _width, _height;
    //contructor
    SpriteComponent(int width = 10, int height = 10)
    {
        this->_width = width; 
        this->_height = height;
    };
};
#endif