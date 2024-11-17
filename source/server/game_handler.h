#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <vector>

#include "event_dispatcher.h"
#include "event_handler.h"
#include "event_queue.h"
#include "logger_s.h"
#include "settings.h"


class GameHandler : public EventHandler, public std::enable_shared_from_this<GameHandler>
{
public:
    GameHandler(EventQueue&, const std::string&);
    ~GameHandler() = default;

    void RegisterInDispatcher(EventDispatcher& d) override;
    boost::asio::awaitable<void> ForwardEventToHandler(ShrdEvent) override;

private:

    template<typename T>
    boost::asio::awaitable<void> HandleClassedEvent(std::shared_ptr<T> event);

    EventQueue& event_queue_;
    const std::string& event_routing_key_;
    std::shared_ptr<Avatar> current_avatar_;
    std::shared_ptr<Location> current_location_;
    std::vector<Npc> npc_container_;


};
