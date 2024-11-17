#pragma once

#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>

#include "event.h"
#include "logger_s.h"

class EventDispatcher;

class EventHandler
{
public:
    using b_await_void = boost::asio::awaitable<void>;

    virtual ~EventHandler() = default;
    
    virtual b_await_void ForwardEventToHandler(ShrdEvent) = 0;
    virtual void RegisterInDispatcher(EventDispatcher& d) = 0;

    /*!
    * @brief PassClassedEvent uses static_pointer_cast: 
    *	- to convert std::shared_ptr<Event> into a derived class, 
    *	- to pass it to a handler-function. 
    *	Event conversion is used to get specific member functions of the event.
    *	For exapmle, event LogPasClientResponse.
    */
    template<typename T>
    b_await_void PassClassedEvent(std::shared_ptr<Event>,
        std::function<b_await_void(std::shared_ptr<T>)>, EventType);
};

#include "event_handler.ipp"


