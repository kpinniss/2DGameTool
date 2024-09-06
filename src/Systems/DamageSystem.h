#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "System.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System
{
    public:
    DamageSystem()
    {
        RequireComponent<BoxColliderComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent& e)
    {
        Logger::Log("DmgSys: Event::Collision detected");
        Logger::Log("Entity A: " + std::to_string(e.a.GetId()));
        Logger::Log("Entity B: " + std::to_string(e.b.GetId()));
        //temp: destroy e on collision
        e.a.Destroy();
    }

    void Update()
    {

    }

};

#endif //DAMAGEEVENT_H