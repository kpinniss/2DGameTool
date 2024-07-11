#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"

int IComponent::_nextId = 0;

int Entity::GetId() const{
    return _id;
}

void System::AddEntity(Entity entity){
    Entities.push_back(entity);
}

void System::RemoveEntity(Entity entity){
    Entities.erase(std::remove_if(Entities.begin(), Entities.end(), [&entity](Entity other) {
        return entity.IsEqual(other);   
    }), Entities.end());
}

std::vector<Entity> System::GetEntities() const{
    return Entities;
}

const SIGNATURE& System::GetSignature() const{
    return ComponentSignature;
}

bool Entity::IsEqual(Entity other_e){
    return _id == other_e.GetId();
}

Entity Registry::CreateEntity() {
    int eId;
    eId = EntityCount++;
    Entity entity(eId);
    EntitiesToCreate.insert(entity);
    if (eId >= EntityComponentSignatures.size()) {
        EntityComponentSignatures.resize(eId + 1);
    }
    Logger::Log("Entity created with id = " + std::to_string(eId));
    return entity;
}

void Registry::AddEntityToSystems(Entity e){
    const auto eId = e.GetId();
    //todo match entityComponentSig to systemsCompSig
    const auto& eCompSig = EntityComponentSignatures[eId];
    for(auto& sys: Systems){
        const auto& sysCompSig = sys.second->GetSignature();
        //bitewise opperation to compare bitsets. If matched add entity
        bool isInterested = (eCompSig & sysCompSig) == sysCompSig;
        if(isInterested){
            sys.second->AddEntity(e);
        }
    }
}

void Registry::Update(){
    //add entities waiting to be created
    for(auto e: EntitiesToCreate){
        AddEntityToSystems(e);
    }
    EntitiesToCreate.clear();
    //TODO: Remove entities that are waiting to be destroyed;
}