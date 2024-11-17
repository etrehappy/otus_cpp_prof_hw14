#include "user_handler.h"
#include "user_handler.ipp" 

UserHandler::UserHandler(EventQueue& q, const std::string& session_id)
    :event_queue_{q}, event_routing_key_{session_id}, uuid_{}
{
}

void UserHandler::RegisterInDispatcher(EventDispatcher& d)
{
try
{
    auto register_handler = [this, &d](EventType type) {
        d.RegisterHandler(type, shared_from_this(), event_routing_key_);
        };

    register_handler(EventType::kFirstConnect);
    register_handler(EventType::kSuccessCheckLogPas);
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}	
} /* RegisterInDispatcher */


boost::asio::awaitable<void> UserHandler::ForwardEventToHandler(ShrdEvent event)
{
try
{
    auto executor = co_await boost::asio::this_coro::executor;

    switch (event->GetType())
    {
    case EventType::kFirstConnect:
        event_queue_.AddEvent(std::make_shared<LoginRequest>(event->GetRoutingKey()));
        /*Does nothing useful in the MVP-version*/
        break;

    case EventType::kSuccessCheckLogPas:
    {
        boost::asio::co_spawn(
            executor,
            PassClassedEvent<SuccessCheckLogPas>(
                event,
                std::bind(&UserHandler::HandleClassedEvent<SuccessCheckLogPas>, this, std::placeholders::_1),
                EventType::kSuccessCheckLogPas
            ),
            boost::asio::detached);
    }
        break;

    case EventType::kFailureCheckLogPas:
        /*Does nothing in the MVP-version. See Networkhendler*/
        break;
    default:
        break;
    }
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    co_return;
}/* ForwardEventToHandler */