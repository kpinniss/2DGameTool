#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H
#include "System.h"
#include "../Events/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControllerComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

class KeyboardControlSystem: public System 
{
    public:
        KeyboardControlSystem() 
        {
            // RequireComponent<KeyboardControllerComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidbodyComponent>();
            RequireComponent<TransformComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) 
        {
            eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
        }

        void OnKeyPressed(KeyPressedEvent& event) 
        {
            for (auto entity: GetEntities()) {
                const auto keyboardcontrol = entity.GetComponent<KeyboardControllerComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();
                auto& rigidbody = entity.GetComponent<RigidbodyComponent>();

                switch (event.symbol) 
                {
                    case SDLK_UP:
                    case SDLK_w:
                        rigidbody.velocity = keyboardcontrol.upVelocity;
                        sprite.srcRect.y = sprite._height * 0;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        rigidbody.velocity = keyboardcontrol.rightVelocity;
                        sprite.srcRect.y = sprite._height * 1;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        rigidbody.velocity = keyboardcontrol.downVelocity;
                        sprite.srcRect.y = sprite._height * 2;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        rigidbody.velocity = keyboardcontrol.leftVelocity;
                        sprite.srcRect.y = sprite._height * 3;
                        break;
                }
            }
        }

        void Update() 
        {
                
        }
};

#endif