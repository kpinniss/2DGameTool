#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "glm.hpp"

struct TransformComponent {
    glm::vec2 Position;
    glm::vec2 Scale;
    double Rotation;
    TransformComponent(
        glm::vec2 pos = glm::vec2(0,0), 
        glm::vec2 scale = glm::vec2(1,1), 
        double rotation = 0.0
        ){
        this->Position = pos;
        this->Rotation = rotation;
        this->Scale = scale;
    }
};

// #include "glm.hpp"

// class IComponent{
//     public:
//         virtual ~IComponent(){};
// };

// class TransformComponent : IComponent
// {
//     public:
//         glm::vec2 Position;
//         glm::vec2 Scale;
//         double Rotation;
//         TransformComponent(){};
//         virtual ~TransformComponent(){};
    
// };

// TransformComponent NewTransformComponent(glm::vec2 position, glm::vec2 scale, double rotation)
// {
//     TransformComponent* c = new TransformComponent();
//     c->Position = position;
//     c->Scale = scale;
//     c->Rotation = rotation;
//     return *c;
// }


#endif
