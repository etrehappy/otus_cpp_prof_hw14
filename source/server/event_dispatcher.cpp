#include "event_dispatcher.h"

EventDispatcher::EventDispatcher()
    : client_handlers_{}
{
}

void EventDispatcher::RegisterHandler(const EventType type
    , std::shared_ptr<EventHandler> handler, const std::string& event_routing_key)
{
try
{
    client_handlers_[event_routing_key][type].push_back(handler);
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* RegisterHandler */

void EventDispatcher::UnregisterHandlers(const std::string& event_routing_key)
{
try
{
    client_handlers_.erase(event_routing_key);
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}    
}/* UnregisterHandlers */

boost::asio::awaitable<void> EventDispatcher::Dispatch(EventQueue& event_queue)
{
    auto executor = co_await boost::asio::this_coro::executor;    
    size_t short_delay = 50;

    while (true)
    {
    try
    {
    
        if (event_queue.IsEmpty())
        {
            boost::asio::steady_timer timer(executor, std::chrono::milliseconds(short_delay));
            co_await timer.async_wait(boost::asio::use_awaitable);
            continue;
        }

        ShrdEvent event = event_queue.ExtractTop();
        EventType type = event->GetType();

        const std::string& event_routing_key = event->GetRoutingKey();
                       
        if (type == EventType::kBroadcastMessage) 
        {
            DispatchBroadcastMessage(event, event_routing_key, executor);
            continue;
        }

        if (type == EventType::kDisconnectSession)
        {
            client_handlers_.erase(event_routing_key);
            continue;
        }
            
        if (!IsHandlerFound(event_routing_key, type))
        {
            continue;
        }

        for (auto& event_handler : client_handlers_[event_routing_key][type])
        {
            boost::asio::co_spawn(executor
                , event_handler->ForwardEventToHandler(event)
                , boost::asio::detached);           
        }  

    }
    catch (const std::exception& e)
    {
        MVP_LOG_EXEPTION(e)
    }
    } /* while */

} /* Dispatch */

void EventDispatcher::DispatchBroadcastMessage(
    ShrdEvent event, const std::string& event_routing_key
    , boost::asio::any_io_executor& executor)
{
try
{
    auto is_valid = [&event_routing_key](const std::string& client_id) 
        { return (client_id != event_routing_key 
            && client_id != settings::server::kDatabaseEventRoutingKey); 
        };

    for (auto& [client_id, handlers_map] : client_handlers_)
    {
        if (!is_valid(client_id)) { continue; }

        for (auto& event_handler : handlers_map[event->GetType()]) 
        {
            boost::asio::co_spawn(executor
                , event_handler->ForwardEventToHandler(event)
                , boost::asio::detached);
        }
       
    }
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* DispatchBroadcastMessage */

bool EventDispatcher::IsHandlerFound(const std::string& event_routing_key
    , EventType type)
{
try
{
    if (client_handlers_.find(event_routing_key) == client_handlers_.end()
        || client_handlers_[event_routing_key].find(type) == client_handlers_[event_routing_key].end())
    {
        logger_s::ErrorHandlerNotFound(type);
        return false;
    }

    return true;
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
    return false;
}    
}/* IsHandlerFound */
