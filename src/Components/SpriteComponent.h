#ifndef SPRIT_COMPONENT_H
#define SPRIT_COMPONENT_H
#include <glm/glm.hpp>
#include <string>

struct SpriteComponent 
{
    int _width, _height;
    std::string _assetId;
    //contructors
    SpriteComponent() = default;
    SpriteComponent(std::string assetId, int width = 10, int height = 10)
    {
        this->_width = width; 
        this->_height = height;
        this->_assetId = assetId;
    };
};
#endif