#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "event.h"
#include "event_handler.h"
#include "event_queue.h"
#include "logger_s.h"

class EventDispatcher 
{
public:
    EventDispatcher();
    ~EventDispatcher() = default;

    void RegisterHandler(const EventType
        , std::shared_ptr<EventHandler>
        , const std::string& event_routing_key);

    void UnregisterHandlers(const std::string& event_routing_key);

    boost::asio::awaitable<void> Dispatch(EventQueue&); 

private:
    using EventHandlerVector = std::vector<std::shared_ptr<EventHandler>>;
    using EventTypeMap = std::unordered_map<EventType, EventHandlerVector>;
    using ClientHandlerMap = std::unordered_map<std::string, EventTypeMap>;

    void DispatchBroadcastMessage(ShrdEvent, const std::string& event_routing_key
        , boost::asio::any_io_executor&);

    bool IsHandlerFound(const std::string& event_routing_key, EventType);

    ClientHandlerMap client_handlers_;
};

