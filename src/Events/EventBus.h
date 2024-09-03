#ifndef EVENT_BUS_H
#define EVENT_BUS_H
#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <list>
#include <memory>
#include <typeindex>
#include <functional>

//EventCallback is a function that takes an event and returns void
class IEventCallback
{
    private:
        virtual void Call(Event& e) = 0;

    public:
        virtual ~IEventCallback() = default;
        
        void Execute(Event& e) 
        {
            Call(e);
        }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
    private:
        typedef void (TOwner::*CallbackFunction)(TEvent&);
        //used to keep  track of the owner of the callback
        TOwner* ownerInstance;
        //callback function that will be invoked when the event is emitted
        CallbackFunction callbackFunction;
        virtual void Call(Event& e) override
        {
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
        }

    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction)
        {
            this->callbackFunction = callbackFunction;
            this->ownerInstance = ownerInstance;
        }
        virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus
{
    private:
        std::map<std::type_index,std::unique_ptr<HandlerList>> subscribers;
    public:
        EventBus()
        {
            Logger::Log("EventBus Created");
        }
        ~EventBus()
        {
            Logger::Log("EventBus Destroyed");
        }

        //Reset the subscribers map
        void Reset()
        {
            subscribers.clear();
        }

        /////////////////////////////////////////////////////////////////////// 
        // Subscribe to an event type <T>
        // In our implementation, a listener subscribes to an event
        // Example: eventBus->SubscribeToEvent<CollisionEvent>(this, &Game::onCollision);
        /////////////////////////////////////////////////////////////////////// 
        template <typename TEvent, typename TOwner>
        void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
            if (!subscribers[typeid(TEvent)].get()) {
                subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
            subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
        }

        /////////////////////////////////////////////////////////////////////// 
        // Emit an event of type <T>
        // In our implementation, as soon as something emits an
        // event we go ahead and execute all the listener callback functions
        // Example: eventBus->EmitEvent<CollisionEvent>(player, enemy);
        /////////////////////////////////////////////////////////////////////// 
        template <typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args) {
            auto handlers = subscribers[typeid(TEvent)].get();
            if (handlers) {
                for (auto it = handlers->begin(); it != handlers->end(); it++) {
                    auto handler = it->get();
                    TEvent event(std::forward<TArgs>(args)...);
                    handler->Execute(event);
                }
            }
        }
};

#endif // EVENT_BUS_H