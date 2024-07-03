#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
struct BaseComponent
{
    private:
        int _nextId;
};

template <typename T> class Component : public BaseComponent
{
 //returns unique id of component
 static int GetId(){
    static auto id = id++;
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
class IPool{
    public:
        virtual ~IPool(){}
};

template <typename T> class Pool : IPool
{
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

        T& Get(int index)()
        {
            return static_cast<T&>(Collection[index]);
        }

        T& operator [](unsigned int index)()
        {
            return Collection[index];
        }
};

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
        std::vector<IPool*> ComponentPools;

        // Vector of component signatures
        // the signature keeps track of which components are active
        // [vector index = entityId]
        std::vector<SIGNATURE> EntityComponentSignatures;

        // map (dictionary) of active systems
        std::unordered_map<std::type_index, System> Systems;

        public:
            Registry() = default;
            
            // TODO:
            void Update();

            //Entity Managment
            Entity CreateEntity();

            //template functions must be described in the header file appose to the cpp file like other functions
            template <typename T, typename ...TArgs> 
            void AddComponent(Entity e, TArgs&& ...args)
            {
                const auto componentId = 
            }

            // manage entities ***
            // CreateEntity()
            // DestroyEntity()
            // AddComponent(Entity ent)
            // RemoveComponent(Entity ent)
            // HasComponent(Entity ent)

            // manage sys ***
            // AddSys()
            // RemoveSys()
            // GetSys()
};

template <typename T> void System::RequireComponent()
{
    const auto compId = Component<T>::GetId();
    ComponentSignature.set(compId);
}
#endif