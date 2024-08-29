#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "System.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <algorithm> // Add this line to include the <algorithm> header

class RenderSystem : public System
{
    public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
    {
        //contain and sort entities by layer

        //create struct to hold entity and its components
        struct RenderableEntity{
            TransformComponent transform;
            SpriteComponent sprite;
        };

        //create vector of renderable entities
        std::vector<RenderableEntity> renderableEntities;

        //populate vector
        for(auto e: GetEntities()){
            RenderableEntity re;
            re.sprite = e.GetComponent<SpriteComponent>();
            re.transform = e.GetComponent<TransformComponent>();
            renderableEntities.emplace_back(re);
        }

        //sort
        std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b){
            return a.sprite.z_index < b.sprite.z_index;
        });
        
       for(auto e: renderableEntities){
            //TODO update entity position
            const TransformComponent& transform = e.transform;
            const SpriteComponent& sprite = e.sprite;
            //create source and destination rects of sprite
            SDL_Rect srcRect = sprite.srcRect;
            SDL_Rect destRect = {
                static_cast<int>(transform.Position.x),
                static_cast<int>(transform.Position.y),
                static_cast<int>(sprite._width * transform.Scale.x),
                static_cast<int>(sprite._height * transform.Scale.y)
            };
            //draw png texture
            SDL_RenderCopyEx(
                renderer, 
                assetStore->GetTexture(sprite._assetId),
                &srcRect,
                &destRect,
                transform.Rotation,
                NULL,
                SDL_FLIP_NONE
            );
        }

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
            Logger::Log("Drawing collider for entity: " + std::to_string(e.GetId()));
        }
    }
};

#endif