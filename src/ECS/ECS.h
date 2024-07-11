#ifndef ECS_H
#define ECS_H
#include <vector>
#include <bitset>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "../Logger/Logger.h"

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
struct IComponent
{
    protected:
        static int _nextId;
};


template <typename T> 
class Component : public IComponent
{
 //returns unique id of component
 static int GetId(){
    static auto id = _nextId++;
    return id;
 }
};

/////////////////
/// ENTITY
/////////////////
class Entity
{
    private:
        int _id;
    public:
        Entity(int id): _id(id){};
        Entity(const Entity& Entity) = default;
        int GetId() const;
        bool IsEqual(Entity other_e);

        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return _id == other._id; }
        bool operator !=(const Entity& other) const { return _id != other._id; }
        bool operator >(const Entity& other) const { return _id > other._id; }
        bool operator <(const Entity& other) const { return _id < other._id; }
};

/////////////////
/// SYSTEM
/////////////////
class System
{
    private:
        SIGNATURE ComponentSignature;
        std::vector<Entity> Entities;
    public:
        System() = default;
        ~System() = default;

        void AddEntity(Entity entity);
        void RemoveEntity(Entity entity);
        std::vector<Entity> GetEntities() const;
        const SIGNATURE& GetSignature() const;
        template <typename T> void RequireComponent();
};

//////////////////
// POOL - a collection of template with accessor functions
/////////////////
class IPool
{
    public:
        virtual ~IPool(){}
};

template <typename T> 
class Pool : IPool {
    
    private:
        std::vector<T> Collection;
    
    public:
        Pool(int size = 100)
        {
            Collection.resize(size);
        }
        virtual ~Pool(){};

        bool IsEmpty()
        {
            return Collection.empty();
        }

        int GetSize() const
        {
            return Collection.size;
        }

        void Resize(int n)
        {
            Collection.resize(n);
        }

        void Clear()
        {
            Collection.clear();
        }

        void Add(T obj)
        {
            Collection.push_back(obj);
        }

        void Set(int index, T obj)
        {
            Collection[index] = obj;
        }

        T& Get(int index)
        {
            return static_cast<T&>(Collection[index]);
        }

        T& operator [](unsigned int index)
        {
            return Collection[index];
        }
};

/////////////////
/// REGISTRY
/////////////////
class Registry {
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

        public:
            Registry() {
                Logger::Log("Registry Created");
            }
            ~Registry() {
                Logger::Log("Registry Destroyed");
            }
            // TODO:
            void Update();

            //Entity Management
            Entity CreateEntity();

            //Component Management
            //template functions must be described in the header file appose to the cpp file like other functions
            template <typename T, typename ...TArgs> void AddComponent(Entity e, TArgs&& ...args);
            template <typename T> void RemoveComponent(Entity e);
            template <typename T> bool HasComponent(Entity e); 

            //Manage Systems
            //Add, Remove, Has, Get
            template <typename T, typename ...TArgs> void AddSystem(TArgs&& ...args);
            template <typename T> void RemoveSystem();
            template <typename T> bool HasSystem() const;
            template <typename T> T GetSystem() const;

            //checks component sig of any entity and adds the entity to Systems
            void AddEntityToSystems(Entity e);
};

///
//Registry Methods
///
template <typename T, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args){
    std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);
    Systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template <typename T> 
void Registry::RemoveSystem(){
    auto sys = Systems.find(std::type_index(typeid(T)));
    Systems.erase(sys);
}

template <typename T> 
bool Registry::HasSystem() const{
    return Systems.find(std::type_index(typeid(T))) != Systems.end();
}

template <typename T> 
T Registry::GetSystem() const{
    auto sys = Systems.find(std::type_index(typeid(T)));
    return *(std::static_pointer_cast<T>(sys->second));
}

template <typename T>
void Registry::RemoveComponent(Entity e){
    const auto compId = Component<T>::GetId();
    const auto entityId = e.GetId();
    EntityComponentSignatures[entityId].set(compId,false);
}

template <typename T>
bool Registry::HasComponent(Entity e){
    const auto compId = Component<T>::GetId();
    const auto entityId = e.GetId();
    return EntityComponentSignatures[entityId].test(compId);
}

template <typename T, typename ...TArgs> 
void Registry::AddComponent(Entity e, TArgs&& ...args){
    const auto compId = Component<T>::GetId();
    const auto entityId = e.GetId();

    if(compId >= ComponentPools.size()){
        ComponentPools.resize(compId + 1, nullptr);
    }
    if(!ComponentPools[compId]){
        std::shared_ptr<Pool<T>> newCompPool = std::make_shared<Pool<T>>();
        ComponentPools[compId] = newCompPool;
    }
    std::shared_ptr<Pool<T>> compPool = std::static_pointer_cast<Pool<T>>(ComponentPools[compId]);
    if(entityId >= compPool.GetSize()){
        compPool->Resize(EntityCount);
    }
    T newComp(std::forward<TArgs>(args)...);
    compPool->Set(entityId,newComp);
    EntityComponentSignatures[entityId].set(compId); 
}

///
//System Methods
///
template <typename T> 
void System::RequireComponent(){
    const auto compId = Component<T>::GetId();
    ComponentSignature.set(compId);
}
#endif