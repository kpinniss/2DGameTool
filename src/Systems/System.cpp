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
    return Entities;
}

const SIGNATURE& System::GetSignature() const
{
    return ComponentSignature;
}