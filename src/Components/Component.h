#ifndef COMPONENT_H
#define COMPONENT_H
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
public:
 static int GetId(){
    static auto id = _nextId++;
    return id;
 }
};
#endif // COMPONENT_H