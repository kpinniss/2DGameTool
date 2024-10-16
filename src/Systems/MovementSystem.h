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
            transform.Position.x = transform.Position.x < 0 ? 0 : transform.Position.x;
            transform.Position.y = transform.Position.y < 0 ? 0 : transform.Position.y;
            transform.Position.x = transform.Position.x >= Game::_mapWidth ? Game::_mapWidth : transform.Position.x;
            transform.Position.y = transform.Position.y >= Game::_mapHeight ? Game::_mapHeight : transform.Position.y;
        }
    }
};

#endif