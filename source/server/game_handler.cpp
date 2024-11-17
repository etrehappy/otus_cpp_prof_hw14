#include "game_handler.h"
#include "game_handler.ipp" 

GameHandler::GameHandler(EventQueue& q, const std::string& session_id)
    :event_queue_{q}, event_routing_key_{session_id}, current_avatar_{nullptr}
    , current_location_{nullptr}, npc_container_{}
{	
}

void GameHandler::RegisterInDispatcher(EventDispatcher& d)
{
try
{
    auto register_handler = [this, &d](EventType type) {
        d.RegisterHandler(type, shared_from_this(), event_routing_key_);
        };

    register_handler(EventType::kStartGameClientRequest);
    register_handler(EventType::kAvatarsDataReady);
    register_handler(EventType::kExitGameRequest);
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* RegisterInDispatcher */

boost::asio::awaitable<void> GameHandler::ForwardEventToHandler(ShrdEvent event)
{
try
{
    auto executor = co_await boost::asio::this_coro::executor;

    switch (event->GetType())
    {
    case EventType::kStartGameClientRequest:
    {
        boost::asio::co_spawn(
            executor,
            PassClassedEvent<StartGameClientRequest>(
                event,
                std::bind(&GameHandler::HandleClassedEvent<StartGameClientRequest>, this, std::placeholders::_1),
                EventType::kStartGameClientRequest
            ),
            boost::asio::detached);
    }
        break;

    case EventType::kAvatarsDataReady:
    {
        boost::asio::co_spawn(
            executor,
            PassClassedEvent<AvatarDataReady>(
                event,
                std::bind(&GameHandler::HandleClassedEvent<AvatarDataReady>, this, std::placeholders::_1),
                EventType::kAvatarsDataReady
            ),
            boost::asio::detached);
    }
        break;

    case EventType::kExitGameRequest:
    {
        boost::asio::co_spawn(
            executor,
            PassClassedEvent<ExitGameRequest>(
                event,
                std::bind(&GameHandler::HandleClassedEvent<ExitGameRequest>, this, std::placeholders::_1),
                EventType::kExitGameRequest
            ),
            boost::asio::detached);
    }
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
