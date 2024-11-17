#include "event_queue.h"

bool EventComparator::operator()(const ShrdEvent& a, const ShrdEvent& b)
{
    return a->GetPriority() < b->GetPriority();
}




EventQueue::EventQueue()
    : queue_{}
{
}

void EventQueue::AddEvent(ShrdEvent event)
{
    queue_.push(event);
}

const bool EventQueue::IsEmpty() const
{
    return queue_.empty();
}

ShrdEvent EventQueue::ExtractTop()
{
    auto t = queue_.top();   
    queue_.pop();
    return t;
}

