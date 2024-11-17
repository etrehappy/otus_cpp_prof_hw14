#pragma once

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 
#include <memory> 

#include "event.h"
#include "event_dispatcher.h"
#include "event_handler.h"
#include "event_queue.h"
#include "game_handler.h"
#include "logger_s.h"
#include "network_handler.h"
#include "settings.h"
#include "user_handler.h"

using BstTcpAcceptor = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::acceptor>;
using BstTcpSocket = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::socket>;

class ClientSession : public std::enable_shared_from_this<ClientSession> 
{
public:
    ClientSession(std::shared_ptr<BstTcpSocket>, EventQueue&, EventDispatcher&);
    ~ClientSession() = default;

    boost::asio::awaitable<void> Start();
    bool IsActive() const;

    /*!
    * @brief Used in NetworkHandler::HandleClassedEvent(std::shared_ptr<ExitGameResponse> event)
    */
    void SetInactive();

private:

    std::shared_ptr<BstTcpSocket> socket_;
    EventQueue& event_queue_;
    EventDispatcher& dispatcher_;
    const std::string client_ip_, session_id_;
    bool active_;
    std::shared_ptr<UserHandler> user_handler_;
    std::shared_ptr<NetworkHandler> network_handler_;
    std::shared_ptr<GameHandler> game_handler_;
};
