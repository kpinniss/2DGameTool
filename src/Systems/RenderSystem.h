#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "System.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>

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
       for(auto e: GetEntities()){
            //TODO update entity position
            const TransformComponent& transform = e.GetComponent<TransformComponent>();
            const SpriteComponent& sprite = e.GetComponent<SpriteComponent>();
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
    }
};

#endif