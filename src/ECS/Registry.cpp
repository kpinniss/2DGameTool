#include "Registry.h"
#include <algorithm>
#include "../Logger/Logger.h"
#include "../Models/Entity.h"
#include "../Systems/System.h"

int IComponent::_nextId = 0;

Entity Registry::CreateEntity()
{
    int eId;

    if(FreeEntityIds.empty())
    {
        //IF no free entity ids
        eId = EntityCount++;
        if (eId >= static_cast<int>(EntityComponentSignatures.size()))
        {
            EntityComponentSignatures.resize(eId + 1);
        }
    }
    else
    {
        eId = FreeEntityIds.front();
    }
    Entity entity(eId);
    entity.registry = this;
    EntitiesToCreate.insert(entity);
    Logger::Log("Entity created with id = " + std::to_string(eId));
    return entity;
}

void Registry::DestroyEntity(Entity e)
{
    const auto eId = e.GetId();
    EntitiesToDestroy.insert(e);
    Logger::Log("Entity destroyed with id = " + std::to_string(eId));
}

void Registry::AddEntityToSystems(Entity e)
{
    const auto eId = e.GetId();
    //todo match entityComponentSig to systemsCompSig
    const auto& eCompSig = EntityComponentSignatures[eId];
    for(auto& sys: Systems)
    {
        const auto& sysCompSig = sys.second->GetSignature();
        //bitewise opperation to compare bitsets. If matched add entity
        bool isInterested = (eCompSig & sysCompSig) == sysCompSig;
        if(isInterested)
        {
            sys.second->AddEntity(e);
        }
    }
}

void Registry::RemoveEntityFromSystems(Entity e)
{
    const auto eId = e.GetId();
    for(auto& sys: Systems)
    {
        sys.second->RemoveEntity(e);
    }
}

void Registry::Update()
{
    //add entities waiting to be created
    for(auto e: EntitiesToCreate)
    {
        AddEntityToSystems(e);
    }
    EntitiesToCreate.clear();
    //TODO: Remove entities that are waiting to be destroyed;
    for(auto e: EntitiesToDestroy)
    {
        //remove entity from systems
        RemoveEntityFromSystems(e);
        //reset entity signature
        EntityComponentSignatures[e.GetId()].reset();
        //capture id for reuse by adding to freeEntityIds
        FreeEntityIds.push_back(e.GetId());
    }
    EntitiesToDestroy.clear();
}