#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <memory>

#include "event_dispatcher.h"
#include "event_handler.h"
#include "event_queue.h"
#include "game_elements.h"
#include "logger_s.h"
#include "messages.pb.h"
#include "settings.h"

using BstTcpAcceptor = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::acceptor>;
using BstTcpSocket = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::socket>;

class ClientSession;

class NetworkHandler : public EventHandler, public std::enable_shared_from_this<EventHandler>
{
public:
    NetworkHandler(std::shared_ptr<BstTcpSocket> s, EventQueue&, const std::string&);
    ~NetworkHandler();
    
    void RegisterInDispatcher(EventDispatcher& d) override;
    boost::asio::awaitable<void> ForwardEventToHandler(ShrdEvent) override;	
    boost::asio::awaitable<void> GetClientResponse(std::shared_ptr<ClientSession> session);

private:
    void ClientSessionDisconnect();
    boost::asio::awaitable<void> HandleEvent(EventType, ProtoMessage::CommandType);

    template<typename T>
    boost::asio::awaitable<void> HandleClassedEvent(std::shared_ptr<T> event);

    std::shared_ptr<BstTcpSocket> socket_;
    EventQueue& event_queue_;
    const std::string& event_routing_key_;
    std::shared_ptr<const std::string> uuid_;
    std::shared_ptr<const Avatar> current_avatar_;
};


