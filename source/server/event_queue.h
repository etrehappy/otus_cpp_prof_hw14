#pragma once

#include <boost/asio.hpp>
#include <queue>

#include "event.h"
#include "logger_s.h"

struct EventComparator 
{
    bool operator()(const ShrdEvent& a, const ShrdEvent& b);
};

class EventQueue
{
public:
    EventQueue();
    ~EventQueue() = default;

    void AddEvent(ShrdEvent event);
    const bool IsEmpty() const;
    ShrdEvent ExtractTop();

private:    
    std::priority_queue<ShrdEvent
        , std::vector<ShrdEvent>, EventComparator> queue_;   
};