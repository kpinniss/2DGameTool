#ifndef RENDER_GIZMOS_SYSTEM_H
#define RENDER_GIZMOS_SYSTEM_H
#include "System.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <algorithm> // Add this line to include the <algorithm> header

class RenderGizmosSystem : public System
{
    public:
    RenderGizmosSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update(SDL_Renderer* renderer)
    {
         auto entities = GetEntities();
            
        for(auto e: entities)
        {
            auto transform = e.GetComponent<TransformComponent>();
            auto collider = e.GetComponent<BoxColliderComponent>();
            SDL_Rect rect = {
                static_cast<int>(transform.Position.x + collider.offset.x),
                static_cast<int>(transform.Position.y + collider.offset.y),
                collider.width,
                collider.height
            };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
            // Logger::Log("Drawing collider for entity: " + std::to_string(e.GetId()));
        }
    }
};

#endif