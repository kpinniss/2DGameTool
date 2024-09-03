#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H
#include <SDL2/SDL.h>
#include "../Models/Entity.h"
#include "Event.h"

class CollisionEvent: public Event
{
    public:
        Entity a;
        Entity b;
        CollisionEvent(Entity a, Entity b): a(a), b(b){}
};
#endif // COLLISIONEVENT_H