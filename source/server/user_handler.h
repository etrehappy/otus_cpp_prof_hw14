#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <memory>

#include "event_dispatcher.h"
#include "event_handler.h"
#include "game_elements.h"
#include "logger_s.h"
#include "settings.h"

/*!
* @brief It can be used to handle a connection status of the user. 
* But in the MVP-version this class hasn't a payload.  
*/
class UserHandler : public EventHandler, public std::enable_shared_from_this<UserHandler>
{
public:
    UserHandler(EventQueue&, const std::string&);
    ~UserHandler() = default;

    boost::asio::awaitable<void> ForwardEventToHandler(ShrdEvent) override;
    void RegisterInDispatcher(EventDispatcher& d) override;

private:

    template<typename T>
    boost::asio::awaitable<void> HandleClassedEvent(std::shared_ptr<T> event);

    EventQueue& event_queue_;
    const std::string& event_routing_key_;
    std::shared_ptr<const std::string> uuid_{};
};


    