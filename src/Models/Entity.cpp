#include "Entity.h"

int Entity::GetId() const
{
    return _id;
}

bool Entity::IsEqual(Entity other_e)
{
    return _id == other_e.GetId();
}