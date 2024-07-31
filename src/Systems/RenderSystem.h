#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "System.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>

class RenderSystem : public System
{
    public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer)
    {
       for(auto e: GetEntities()){
            //TODO update entity position
            const TransformComponent& transform = e.GetComponent<TransformComponent>();
            const SpriteComponent& sprite = e.GetComponent<SpriteComponent>();

            SDL_Rect objectToRender = {
                static_cast<int>(transform.Position.x),
                static_cast<int>(transform.Position.y),
                sprite._width,
                sprite._height
            };
            SDL_SetRenderDrawColor(renderer, 200,0,255,255);
            SDL_RenderFillRect(renderer, &objectToRender);
            //log
            // Logger::Log
            // (
            //     "Entity id: "+ std::to_string(e.GetId()) +
            //     "Position: (x: " + std::to_string(transform.Position.x) +
            //     " y: " +std::to_string(+transform.Position.y ) +
            //     ")"
            //   );
        }
    }
};

#endif