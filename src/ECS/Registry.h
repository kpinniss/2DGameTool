#ifndef REGISTRY_H
#define REGISTRY_H
#include <vector>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "../Logger/Logger.h"
#include "../Components/Component.h"
#include "../Models/Entity.h"
#include "../Models/Pool.h"
#include "../Models/Util.h"
#include "../Systems/System.h"
#include <deque>
class System;
/////////////////
/// REGISTRY
/////////////////
class Registry 
{
    private:
        int EntityCount = 0;

        // Vector component pools
        // Each pool contains all the data for a certain component type
        // [vector index = componentId], [pool index = entity]
        std::vector<std::shared_ptr<IPool>> ComponentPools;

        // Vector of component signatures
        // the signature keeps track of which components are active
        // [vector index = entityId]
        std::vector<SIGNATURE> EntityComponentSignatures;

        // map (dictionary) of active systems
        std::unordered_map<std::type_index, std::shared_ptr<System>> Systems;

        // Set of entities that are flagged to be added or removed in the next registry Update()
        std::set<Entity> EntitiesToCreate;
        std::set<Entity> EntitiesToDestroy;

        // Queue of free entity ids (previously destroyed entities)
        std::deque<int> FreeEntityIds;

        public:
            Registry() 
            {
                Logger::Log("Registry Created");
            }
            ~Registry() 
            {
                Logger::Log("Registry Destroyed");
            }
            // TODO:
            void Update();

            //Entity Management
            Entity CreateEntity();
            void DestroyEntity(Entity e);
            //Component Management
            //template functions must be described in the header file appose to the cpp file like other functions
            template <typename T, typename ...TArgs> void AddComponent(Entity e, TArgs&& ...args);
            template <typename T> void RemoveComponent(Entity e);
            template <typename T> bool HasComponent(Entity e); 
            template <typename T> T& GetComponent(Entity entity) const;
            
            //Manage Systems
            //Add, Remove, Has, Get
            template <typename T, typename ...TArgs> void AddSystem(TArgs&& ...args);
            template <typename T> void RemoveSystem();
            template <typename T> bool HasSystem() const;
            template <typename T> T& GetSystem() const;
            //checks component sig of any entity and adds the entity to Systems
            void AddEntityToSystems(Entity e);
            void RemoveEntityFromSystems(Entity e);
};

///
//Registry Methods
///
template <typename T, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args)
{
    std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);
    Systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template <typename T> 
void Registry::RemoveSystem(){
    auto sys = Systems.find(std::type_index(typeid(T)));
    Systems.erase(sys);
}

template <typename T> 
bool Registry::HasSystem() const
{
    return Systems.find(std::type_index(typeid(T))) != Systems.end();
}

template <typename T> 
T& Registry::GetSystem() const
{
    auto sys = Systems.find(std::type_index(typeid(T)));
    return *(std::static_pointer_cast<T>(sys->second));
}

template <typename T>
void Registry::RemoveComponent(Entity e)
{
    const auto compId = Component<T>::GetId();
    const auto entityId = e.GetId();
    EntityComponentSignatures[entityId].set(compId,false);
}

template <typename T>
bool Registry::HasComponent(Entity e)
{
    const auto compId = Component<T>::GetId();
    const auto entityId = e.GetId();
    return EntityComponentSignatures[entityId].test(compId);
}

template <typename T, typename ...TArgs> 
void Registry::AddComponent(Entity e, TArgs&& ...args)
{
    const auto compId = Component<T>::GetId();
    const auto entityId = e.GetId();

    if(compId >= static_cast<int>(ComponentPools.size())){
        ComponentPools.resize(compId + 1, nullptr);
    }
    if(!ComponentPools[compId]){
        std::shared_ptr<Pool<T>> newCompPool = std::make_shared<Pool<T>>();
        ComponentPools[compId] = std::static_pointer_cast<IPool>(newCompPool);
    }
    std::shared_ptr<Pool<T>> compPool = std::static_pointer_cast<Pool<T>>(ComponentPools[compId]);
    if(entityId >= compPool->GetSize()){
        compPool->Resize(EntityCount);
    }
    T newComp(std::forward<TArgs>(args)...);
    compPool->Set(entityId,newComp);
    EntityComponentSignatures[entityId].set(compId); 
    Logger::Log("Component Added. ID: " + std::to_string(compId) + " Entity: " + std::to_string(entityId));
}

template <typename T>
T& Registry::GetComponent(Entity entity) const 
{
	const auto componentId = Component<T>::GetId();
	const auto entityId = entity.GetId();
    auto componentPool = std::static_pointer_cast<Pool<T>>(ComponentPools[componentId]);
    return componentPool->Get(entityId);
}

#endif