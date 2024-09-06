#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H
#include "System.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"

class MovementSystem : public System
{
    public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidbodyComponent>();
    }

    void Update(double deltaTime)
    {
        for(auto e: GetEntities()){
            //TODO update entity position
            TransformComponent& transform = e.GetComponent<TransformComponent>();
            const RigidbodyComponent& r_body = e.GetComponent<RigidbodyComponent>();
            transform.Position.x += r_body.velocity.x * deltaTime;
            transform.Position.y += r_body.velocity.y * deltaTime;

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