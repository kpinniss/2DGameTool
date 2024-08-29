#ifndef SPRIT_COMPONENT_H
#define SPRIT_COMPONENT_H
#include <glm/glm.hpp>
#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent 
{
    int _width, _height, z_index;
    std::string _assetId;
    SDL_Rect srcRect;
    //contructors
    SpriteComponent() = default;
    SpriteComponent(
        std::string assetId, 
        int width = 10, 
        int height = 10,
        int zindex = 0,
        int srcRectX = 0,
        int srcRectY = 0
    )
    {
        this->_width = width; 
        this->_height = height;
        this->z_index = zindex;
        this->_assetId = assetId;
        this->srcRect = {srcRectX, srcRectY, width, height};
    };
};
#endif