

template<class T>
boost::asio::awaitable<void> EventHandler::PassClassedEvent(
    std::shared_ptr<Event> event,
    std::function<boost::asio::awaitable<void>(std::shared_ptr<T>)> handler_func,
    EventType type)
{
try
{
    auto executor = co_await boost::asio::this_coro::executor;

    auto classed_event = std::static_pointer_cast<T>(event);

    if (!classed_event)
    {
        logger_s::ErrorDynamicPointerCast(type);
        co_return;
    }

    co_await handler_func(classed_event);
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    co_return;
}