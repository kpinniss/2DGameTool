#ifndef CAMERA_SYSTEM_H
#include "System.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../ECS/Registry.h"

class CameraSystem: public System
{

//TODO: Implement CameraSystem
public:

    CameraSystem()
    {
        RequireComponent<CameraComponent>();
        RequireComponent<TransformComponent>();

        Logger::Log("CameraSystem constructor called.");
    }

    void Update(SDL_Rect& camera)
    {
        for(auto e: GetEntities())
        {
            auto transform = e.GetComponent<TransformComponent>();
            //update camera postion to follow the entity
            if((transform.Position.x + (camera.w / 2)) < Game::_mapWidth){
                camera.x = transform.Position.x - camera.w / 2;;
            }
            if((transform.Position.y + (camera.h / 2)) < Game::_mapHeight){
                camera.y = transform.Position.y - camera.h / 2;;
            }

            camera.x = camera.x < 0 ? 0 : camera.x;
            camera.x = camera.x > Game::_mapWidth ? Game::_mapWidth : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y;
            camera.y = camera.y > Game::_mapHeight ? Game::_mapHeight : camera.y;

            Logger::Log("Camera postion X: " + std::to_string(camera.x) + " Y: " + std::to_string(camera.y));
        }
    }

};

#endif