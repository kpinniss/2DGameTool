#ifndef ENTITY_H
#define ENTITY_H

/////////////////
/// ENTITY
/////////////////
class Registry;

class Entity
{
    private:
        int _id;
    public:
        Entity(int id): _id(id){};
        Entity(const Entity& Entity) = default;
        void Destroy();
        int GetId() const;
        bool IsEqual(Entity other_e);

        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return _id == other._id; }
        bool operator !=(const Entity& other) const { return _id != other._id; }
        bool operator >(const Entity& other) const { return _id > other._id; }
        bool operator <(const Entity& other) const { return _id < other._id; }
        
        template <typename T, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template <typename T> void RemoveComponent();
        template <typename T> bool HasComponent() const;
        template <typename T> T& GetComponent();

        //Parent registry
        /**
         * Pointer to the registry that manages the entities in the game.
         */
        Registry* registry;
};

#include "../ECS/Registry.h"

template <typename T, typename ...TArgs> 
void Entity::AddComponent(TArgs&& ...args)
{
    registry->template AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template <typename T>
void Entity::RemoveComponent()
{
    registry->template RemoveComponent<T>(*this);
}

template <typename T>
bool Entity::HasComponent() const
{
    return registry->template HasComponent<T>(*this);
}

template <typename T> 
T& Entity::GetComponent()
{
    return registry->template GetComponent<T>(*this);
}
#endif // ENTITY_H
