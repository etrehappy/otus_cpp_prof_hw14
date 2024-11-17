#include "main.h"


boost::asio::awaitable<void> ListenConnect(EventQueue& q, EventDispatcher& d)
{
    auto executor = co_await boost::asio::this_coro::executor;

    BstTcpAcceptor acceptor(executor, {boost::asio::ip::tcp::v4(), settings::server::kPort});

    while (true)
    {
        auto socket = std::make_shared<BstTcpSocket>(co_await acceptor.async_accept());
        
        try
        {
            auto session = std::make_shared<ClientSession>(socket, q, d);
            boost::asio::co_spawn(executor, session->Start(), boost::asio::detached);
        }
        catch (const std::exception& e)
        {
            MVP_LOG_EXEPTION(e)
        }
    }
}

int main() 
{
#ifdef _WIN32    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale("ru_RU.UTF-8"));
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif

    logger_s::InitLogging();

try
{
    boost::asio::io_context io_context{};

    EventQueue event_queue{};

    EventDispatcher dispatcher{};

    Database database{};
    auto db_handler = std::make_shared<DatabaseHandler>(database, event_queue);
    db_handler->RegisterInDispatcher(dispatcher);

    boost::asio::co_spawn(io_context, dispatcher.Dispatch(event_queue), boost::asio::detached);

    boost::asio::co_spawn(io_context, ListenConnect(event_queue, dispatcher), boost::asio::detached);

    logger_s::ServerStart();

    io_context.run();
}
catch (const std::exception& e) 
{
    MVP_LOG_EXEPTION(e)
}   
    return 0;
}