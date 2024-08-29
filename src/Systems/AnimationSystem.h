#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "System.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include <SDL2/SDL.h>

class AnimationSystem: public System {
    public:
        AnimationSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<AnimationComponent>();
        }

        void Update() {
            for (auto entity: GetEntities()) {
                auto& animation = entity.GetComponent<AnimationComponent>();
                auto& spriteComponent = entity.GetComponent<SpriteComponent>();

                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
                spriteComponent.srcRect.x = animation.currentFrame * spriteComponent._width;
            }
        }
};

#endif