#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "System.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"

struct BoxCollider{
    int x, y, w, h;
    BoxCollider(int x, int y, int w, int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

class CollisionSystem : public System
{
    public:
    CollisionSystem()
    {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(std::unique_ptr<EventBus>& eventBus)
    {
        auto entities = GetEntities();
       //loop over each entity and check for collisions
       for(auto i = entities.begin(); i != entities.end(); i++)
       {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxColliderComponent>();
            //loop entities that havent been checked.
            for(auto j = i; j != entities.end(); j++)
            {
                Entity b = *j;
                if(a == b) continue;
                //check for collision between a and b
                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxColliderComponent>();
                BoxCollider boxA = BoxCollider(
                    aTransform.Position.x + aCollider.offset.x, 
                    aTransform.Position.y + aCollider.offset.y, 
                    aCollider.width, aCollider.height
                );
                BoxCollider boxB = BoxCollider(
                    bTransform.Position.x + bCollider.offset.x, 
                    bTransform.Position.y + bCollider.offset.y, 
                    bCollider.width, bCollider.height
                );
                if(CheckCollision(boxA, boxB))
                {
                    // a.Destroy();
                    // b.Destroy();
                    Logger::Log("Collision detected");
                    Logger::Log("Entity A: " + std::to_string(a.GetId()));
                    Logger::Log("Entity B: " + std::to_string(b.GetId()));

                    eventBus->EmitEvent<CollisionEvent>(a,b);
                }
            }
       }
    }

    bool CheckCollision(BoxCollider a, BoxCollider b){
        bool collision = (a.x < b.x + b.w &&
                          a.x + a.w > b.x &&
                          a.y < b.y + b.h &&
                          a.y + a.h > b.y);
        return collision;
    }
};

#endif