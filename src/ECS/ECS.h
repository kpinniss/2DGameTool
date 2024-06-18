#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;
/////////////////
/// SIGNATURE
/// Used to keep track of which components an entity has
/// also of which entities a system is interested in
////////////////
typedef std::bitset<MAX_COMPONENTS> SIGNATURE;

/////////////////
/// COMPONENT
/////////////////
class Component{

};

/////////////////
/// ENTITY
/////////////////
class Entity{
    private:
        int _id;
    public:
        Entity(int id): _id(id){};
        int GetId() const;
};

/////////////////
/// SYSTEM
/////////////////
class System{
    private:
        SIGNATURE ComponentSignature;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;

        void AddEntity(Entity entity);
        void RemoveEntity(Entity entity);
        std::vector<Entity> GetEntities() const;
        SIGNATURE& GetSignature() const;
};

/////////////////
/// REGISTRY
/////////////////
class Registry{

};

#endif