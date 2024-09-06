#include "System.h"
#include "../Models/Entity.h"
#include <algorithm>
#include <vector>

void System::AddEntity(Entity entity)
{
    Entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
    Entities.erase(std::remove_if(Entities.begin(), Entities.end(), [&entity](Entity other) 
    {
        return entity.IsEqual(other);   
    }), Entities.end());
}

std::vector<Entity> System::GetEntities() const
{
    
    // if(Entities.size() > 10000)
    // {
    //     Logger::Err("Entities size is too large, exiting...");
    //     exit(1);
    // }
    Logger::Log("Entities size: " + std::to_string(Entities.size()));
    return Entities;
}

const SIGNATURE& System::GetSignature() const
{
    return ComponentSignature;
}