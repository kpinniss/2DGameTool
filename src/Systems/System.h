#ifndef SYSTEM_H
#define SYSTEM_H
#include <bitset>
#include "../Models/Entity.h"
#include <vector> // Add this line to include the vector header
#include "../Models/Util.h" // Add this line to include the Util header

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

///
//System Methods
///
template <typename T> 
void System::RequireComponent(){
    const auto compId = Component<T>::GetId();
    ComponentSignature.set(compId);
}
#endif // ECSSYSTEM_H